#include <thread>

#include "MovieContainer.h"
#include "core/assets/asset_globals.h"
#include "core/library/library_globals.h"

namespace Library::Containers {
    MovieContainer::MovieContainer() {}
    MovieContainer::MovieContainer(std::string p_source, std::string p_path, bool auto_load, bool* p_found) {
        if (auto_load){
            bool found = loadFromDb(p_source, p_path);
            if (p_found != NULL) {
                *p_found = found;
            }
        } else {
            source = p_source;
            path = p_path;
        }
    }

    MovieContainer::MovieContainer(std::string p_source, std::string p_path, std::string p_title, std::string p_date, std::string p_desc, std::string p_poster_path) {
        source = p_source;
        path = p_path;
        title = p_title;
        date = p_date;
        desc = p_desc;
        poster_path = p_poster_path;
    }
    MovieContainer::~MovieContainer() {}

    // Return true it load was successful:
    bool MovieContainer::reloadFromDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT title, date, desc, poster_path FROM movie WHERE source = ? AND path = ?;");
        sqlite3_bind_text(stmt, 1, source.c_str(), source.length(), NULL);
        sqlite3_bind_text(stmt, 2, path.c_str(), path.length(), NULL);
        
        bool found = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            bool found = true;

            title = (char*)sqlite3_column_text(stmt, 0);
            date = (char*)sqlite3_column_text(stmt, 1);
            desc = (char*)sqlite3_column_text(stmt, 2);
            poster_path = (char*)sqlite3_column_text(stmt, 3);
        }

        sqlite3_finalize(stmt);

        return found;
    }
    bool MovieContainer::loadFromDb(std::string p_source, std::string p_path) {
        source = p_source;
        path = p_path;

        return reloadFromDb();
    }

    bool MovieContainer::existsInDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT COUNT(*) FROM movie WHERE source = ? AND path = ?;");
        // sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT COUNT(*) FROM movie;");
        sqlite3_bind_text(stmt, 1, source.c_str(), source.length(), NULL);
        sqlite3_bind_text(stmt, 2, path.c_str(), path.length(), NULL);
        
        int count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        return count > 0;
    }

    bool MovieContainer::writeToDb() {
        if (existsInDb()) {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "UPDATE movie SET title = ?, date = ?, desc = ?, poster_path = ? WHERE source = ? AND path = ?;"
            );
            
            sqlite3_bind_text(stmt, 1, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 2, date.c_str(), date.length(), NULL);
            sqlite3_bind_text(stmt, 3, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 4, poster_path.c_str(), poster_path.length(), NULL);
            sqlite3_bind_text(stmt, 5, source.c_str(), source.length(), NULL);
            sqlite3_bind_text(stmt, 6, path.c_str(), path.length(), NULL);

            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;

        } else {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "INSERT INTO movie (source, path, title, date, desc, poster_path) VALUES (?, ?, ?, ?, ?, ?);"
            );
            sqlite3_bind_text(stmt, 1, source.c_str(), source.length(), NULL);
            sqlite3_bind_text(stmt, 2, path.c_str(), path.length(), NULL);
            sqlite3_bind_text(stmt, 3, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 4, date.c_str(), date.length(), NULL);
            sqlite3_bind_text(stmt, 5, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 6, poster_path.c_str(), poster_path.length(), NULL);
            
            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;
        }
    }


    void MovieContainer::loadPosterImg() {
        poster_img = Global::asset_manager->loadTexture(poster_path);
    }

}