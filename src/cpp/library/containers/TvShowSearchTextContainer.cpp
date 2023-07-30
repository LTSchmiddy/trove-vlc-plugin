#include <thread>

#include "TvShowSearchTextContainer.h"
#include "assets/asset_globals.h"
#include "library/library_globals.h"

namespace Library::Containers {
    TvShowSearchTextContainer::TvShowSearchTextContainer() {}
    TvShowSearchTextContainer::TvShowSearchTextContainer(std::string p_search_text, bool auto_load, bool* p_found) {
        if (auto_load){
            bool found = loadFromDb(p_search_text);
            if (p_found != NULL) {
                *p_found = found;
            }
        } else {
            search_text = p_search_text;
        }
    }

    TvShowSearchTextContainer::TvShowSearchTextContainer(std::string p_search_text, std::string p_show_title, std::string p_show_date, std::string p_scraper_info) {
        search_text = p_search_text;
        show_title = p_show_title;
        show_date = p_show_date;
        scraper_vars = p_scraper_info;
    }
    TvShowSearchTextContainer::~TvShowSearchTextContainer() {}

    // Return true it load was successful:
    bool TvShowSearchTextContainer::reloadFromDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT show_title, show_date, scraper_vars FROM tv_show_search_text WHERE search_text = ?;");
        sqlite3_bind_text(stmt, 1, search_text.c_str(), search_text.length(), NULL);
        
        bool found = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            bool found = true;

            show_title = (char*)sqlite3_column_text(stmt, 0);
            show_date = (char*)sqlite3_column_text(stmt, 1);
            scraper_vars = (char*)sqlite3_column_text(stmt, 2);
        }

        sqlite3_finalize(stmt);

        return found;
    }
    bool TvShowSearchTextContainer::loadFromDb(std::string p_search_text) {
        search_text = p_search_text;
        return reloadFromDb();
    }

    bool TvShowSearchTextContainer::existsInDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT COUNT(*) FROM tv_show_search_text WHERE search_text = ?");
        sqlite3_bind_text(stmt, 1, search_text.c_str(), search_text.length(), NULL);
        
        int count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        return count > 0;
    }

    bool TvShowSearchTextContainer::writeToDb() {
        if (existsInDb()) {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "UPDATE tv_show_search_text SET show_title = ?, show_date = ?, scraper_vars = ? WHERE search_text = ?;"
            );

            sqlite3_bind_text(stmt, 1, show_title.c_str(), show_title.length(), NULL);
            sqlite3_bind_text(stmt, 2, show_date.c_str(), show_date.length(), NULL);
            sqlite3_bind_text(stmt, 3, scraper_vars.c_str(), scraper_vars.length(), NULL);
            sqlite3_bind_text(stmt, 4, search_text.c_str(), search_text.length(), NULL);

            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;

        } else {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "INSERT INTO tv_show_search_text (search_text, show_title, show_date, scraper_vars) VALUES (?, ?, ?, ?);"
            );
            sqlite3_bind_text(stmt, 1, search_text.c_str(), search_text.length(), NULL);
            sqlite3_bind_text(stmt, 2, show_title.c_str(), show_title.length(), NULL);
            sqlite3_bind_text(stmt, 3, show_date.c_str(), show_date.length(), NULL);
            sqlite3_bind_text(stmt, 4, scraper_vars.c_str(), scraper_vars.length(), NULL);
            
            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;
        }
    }
    
    TvShowContainer TvShowSearchTextContainer::getTvShow(bool* p_found) {
        return TvShowContainer(show_title, show_date, true, p_found);
    }
}