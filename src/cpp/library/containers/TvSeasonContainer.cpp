#include <thread>

#include "TvSeasonContainer.h"
#include "assets/asset_globals.h"
#include "library/library_globals.h"

namespace Library::Containers {
    TvSeasonContainer::TvSeasonContainer() {}
    TvSeasonContainer::TvSeasonContainer(std::string p_title, std::string p_date, int p_season, bool auto_load, bool* p_found) {
        if (auto_load){
            bool found = loadFromDb(p_title, p_date, p_season);
            if (p_found != NULL) {
                *p_found = found;
            }
        } else {
            show_title = p_title;
            show_date = p_date;
            season = p_season;
        }
    }

    TvSeasonContainer::TvSeasonContainer(std::string p_show_title, std::string p_show_date, int p_season, std::string p_title, std::string p_desc, std::string p_poster_path) {
        show_title = p_show_title;
        show_date = p_show_date;
        season = p_season;
        title = p_title;
        desc = p_desc;
        poster_path = p_poster_path;
    }
    TvSeasonContainer::~TvSeasonContainer() {}

    // Return true it load was successful:
    bool TvSeasonContainer::reloadFromDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT title, desc, poster_path FROM tv_season WHERE show_title = ? AND show_date = ? AND season = ?;");
        sqlite3_bind_text(stmt, 1, show_title.c_str(), show_title.length(), NULL);
        sqlite3_bind_text(stmt, 2, show_date.c_str(), show_date.length(), NULL);
        sqlite3_bind_int(stmt, 3, season);
        
        bool found = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            bool found = true;

            title = (char*)sqlite3_column_text(stmt, 0);
            desc = (char*)sqlite3_column_text(stmt, 1);
            poster_path = (char*)sqlite3_column_text(stmt, 2);
        }

        sqlite3_finalize(stmt);

        return found;
    }
    bool TvSeasonContainer::loadFromDb(std::string p_title, std::string p_date, int p_season) {
        show_title = p_title;
        show_date = p_date;
        season = p_season;

        return reloadFromDb();
    }

    bool TvSeasonContainer::existsInDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT COUNT(*) FROM tv_season WHERE show_title = ? AND show_date = ? AND season = ?");
        sqlite3_bind_text(stmt, 1, show_title.c_str(), show_title.length(), NULL);
        sqlite3_bind_text(stmt, 2, show_date.c_str(), show_date.length(), NULL);
        sqlite3_bind_int(stmt, 3, season);
        
        int count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        return count > 0;
    }

    bool TvSeasonContainer::writeToDb() {
        if (existsInDb()) {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "UPDATE tv_season SET title = ?, desc = ?, poster_path = ? WHERE show_title = ? AND show_date = ? AND season = ?;"
            );

            sqlite3_bind_text(stmt, 1, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 2, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 3, poster_path.c_str(), poster_path.length(), NULL);
            sqlite3_bind_text(stmt, 4, show_title.c_str(), show_title.length(), NULL);
            sqlite3_bind_text(stmt, 5, show_date.c_str(), show_date.length(), NULL);
            sqlite3_bind_int(stmt, 6, season);

            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;

        } else {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "INSERT INTO tv_season (show_title, show_date, season, title, desc, poster_path) VALUES (?, ?, ?, ?, ?, ?);"
            );
            sqlite3_bind_text(stmt, 1, show_title.c_str(), show_title.length(), NULL);
            sqlite3_bind_text(stmt, 2, show_date.c_str(), show_date.length(), NULL);
            sqlite3_bind_int(stmt, 3, season);
            sqlite3_bind_text(stmt, 4, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 5, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 6, poster_path.c_str(), poster_path.length(), NULL);
            
            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;
        }
    }
    
    TvShowContainer TvSeasonContainer::getTvShow(bool* p_found) {
        return TvShowContainer(show_title, show_date, true, p_found);
    }

#ifdef UI_BUILD
    void TvSeasonContainer::loadPosterImg() {
        poster_img = Global::asset_manager->loadTexture(poster_path);
    }
#endif
}