#include <thread>

#include <plog/Log.h>

#include "TvShowContainer.h"
#include "core/assets/asset_globals.h"
#include "core/library/library_globals.h"

namespace Library::Containers {
    TvShowContainer::TvShowContainer() {}
    TvShowContainer::TvShowContainer(std::string p_title, std::string p_date, bool auto_load, bool* p_found) {
        if (auto_load){
            bool found = loadFromDb(p_title, p_date);
            if (p_found != NULL) {
                *p_found = found;
            }
        }
        else {
            title = p_title;
            date = p_date;
        }
    }

    TvShowContainer::TvShowContainer(std::string p_title, std::string p_date, int p_seasons, std::string p_desc, std::string p_poster_path) {
        title = p_title;
        seasons = p_seasons;
        date = p_date;
        desc = p_desc;
        poster_path = p_poster_path;
    }
    TvShowContainer::~TvShowContainer() {}

    // Return true it load was successful:
    bool TvShowContainer::reloadFromDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT seasons, desc, poster_path FROM tv_show WHERE title = ? AND date = ?;");
        sqlite3_bind_text(stmt, 1, title.c_str(), title.length(), NULL);
        sqlite3_bind_text(stmt, 2, date.c_str(), date.length(), NULL);
        
        bool found = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            bool found = true;

            seasons = sqlite3_column_int(stmt, 0);
            desc = (char*)sqlite3_column_text(stmt, 1);
            poster_path = (char*)sqlite3_column_text(stmt, 2);
        }

        sqlite3_finalize(stmt);

        return found;
    }
    bool TvShowContainer::loadFromDb(std::string p_title, std::string p_date) {
        title = p_title;
        date = p_date;

        return reloadFromDb();
    }

    bool TvShowContainer::existsInDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT COUNT(*) FROM tv_show WHERE title = ? AND date = ?");
        sqlite3_bind_text(stmt, 1, title.c_str(), title.length(), NULL);
        sqlite3_bind_text(stmt, 2, date.c_str(), date.length(), NULL);
        
        int count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        return count > 0;
    }

    bool TvShowContainer::writeToDb() {
        if (existsInDb()) {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "UPDATE tv_show SET seasons = ?, desc = ?, poster_path = ? WHERE title = ? AND date = ?;"
            );
            
            sqlite3_bind_int(stmt, 1, seasons);
            sqlite3_bind_text(stmt, 2, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 3, poster_path.c_str(), poster_path.length(), NULL);
            sqlite3_bind_text(stmt, 4, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 5, date.c_str(), date.length(), NULL);

            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;

        } else {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "INSERT INTO tv_show (title, date, seasons, desc, poster_path) VALUES (?, ?, ?, ?, ?);"
            );
            sqlite3_bind_text(stmt, 1, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 2, date.c_str(), date.length(), NULL);
            sqlite3_bind_int(stmt, 3, seasons);
            sqlite3_bind_text(stmt, 4, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 5, poster_path.c_str(), poster_path.length(), NULL);
            
            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;
        }
    }

#ifdef UI_BUILD
    void TvShowContainer::loadPosterImg() {
        poster_img = Global::asset_manager->loadTexture(poster_path);
    }
#endif
}