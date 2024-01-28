#include <thread>

#include "TvEpisodeContainer.h"
#include "core/assets/asset_globals.h"
#include "core/library/library_globals.h"

namespace Library::Containers {
    TvEpisodeContainer::TvEpisodeContainer() {}
    TvEpisodeContainer::TvEpisodeContainer(std::string p_source, std::string p_path, bool auto_load, bool* p_found) {
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

    TvEpisodeContainer::TvEpisodeContainer(
            std::string p_source,
            std::string p_path,
            std::string p_title,
            std::string p_show_title,
            std::string p_show_date,
            int p_season,
            int p_episode,
            std::string p_air_date,
            std::string p_desc,
            std::string p_poster_path) {
        source = p_source;
        path = p_path;
        title = p_title;
        show_title = p_show_title;
        show_date = p_show_date;
        season = p_season;
        episode = p_episode;
        air_date = p_air_date;
        desc = p_desc;
        poster_path = p_poster_path;
    }
    TvEpisodeContainer::~TvEpisodeContainer() {}

    // Return true it load was successful:
    bool TvEpisodeContainer::reloadFromDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
            "SELECT title, show_title, show_date, season, episode, air_date, desc, poster_path FROM tv_episode WHERE source = ? AND path = ?;");
        sqlite3_bind_text(stmt, 1, source.c_str(), source.length(), NULL);
        sqlite3_bind_text(stmt, 2, path.c_str(), path.length(), NULL);
        
        bool found = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            bool found = true;

            title = (char*)sqlite3_column_text(stmt, 0);
            show_title = (char*)sqlite3_column_text(stmt, 1);
            show_date = (char*)sqlite3_column_text(stmt, 2);
            season = sqlite3_column_int(stmt, 3);
            episode = sqlite3_column_int(stmt, 4);
            air_date = (char*)sqlite3_column_text(stmt, 5);
            desc = (char*)sqlite3_column_text(stmt, 6);
            poster_path = (char*)sqlite3_column_text(stmt, 7);
        }

        sqlite3_finalize(stmt);

        return found;
    }
    bool TvEpisodeContainer::loadFromDb(std::string p_source, std::string p_path) {
        source = p_source;
        path = p_path;

        return reloadFromDb();
    }

    bool TvEpisodeContainer::existsInDb() {
        sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString("SELECT COUNT(*) FROM tv_episode WHERE source = ? AND path = ?;");
        sqlite3_bind_text(stmt, 1, source.c_str(), source.length(), NULL);
        sqlite3_bind_text(stmt, 2, path.c_str(), path.length(), NULL);
        
        int count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        return count > 0;
    }

    bool TvEpisodeContainer::writeToDb() {
        if (existsInDb()) {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "UPDATE tv_episode SET title = ?, show_title = ?, show_date = ?, season = ?, episode = ?, air_date = ?, desc = ?, poster_path = ? WHERE source = ? AND path = ?;"
            );
            
            sqlite3_bind_text(stmt, 1, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 2, show_title.c_str(), show_title.length(), NULL);
            sqlite3_bind_text(stmt, 3, show_date.c_str(), show_date.length(), NULL);
            sqlite3_bind_int(stmt, 4, season);
            sqlite3_bind_int(stmt, 5, episode);
            sqlite3_bind_text(stmt, 6, air_date.c_str(), air_date.length(), NULL);
            sqlite3_bind_text(stmt, 7, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 8, poster_path.c_str(), poster_path.length(), NULL);
            sqlite3_bind_text(stmt, 9, source.c_str(), source.length(), NULL);
            sqlite3_bind_text(stmt, 10, path.c_str(), path.length(), NULL);

            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;

        } else {
            sqlite3_stmt* stmt = Global::library_db->simpleStatementFromString(
                "INSERT INTO tv_episode (source, path, title, show_title, show_date, season, episode, air_date, desc, poster_path) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
            );
            sqlite3_bind_text(stmt, 1, source.c_str(), source.length(), NULL);
            sqlite3_bind_text(stmt, 2, path.c_str(), path.length(), NULL);
            sqlite3_bind_text(stmt, 3, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(stmt, 4, show_title.c_str(), show_title.length(), NULL);
            sqlite3_bind_text(stmt, 5, show_date.c_str(), show_date.length(), NULL);
            sqlite3_bind_int(stmt, 6, season);
            sqlite3_bind_int(stmt, 7, episode);
            sqlite3_bind_text(stmt, 8, air_date.c_str(), air_date.length(), NULL);
            sqlite3_bind_text(stmt, 9, desc.c_str(), desc.length(), NULL);
            sqlite3_bind_text(stmt, 10, poster_path.c_str(), poster_path.length(), NULL);
            
            bool retVal = Global::library_db->simpleRunStatement(stmt);
            sqlite3_finalize(stmt);
            return retVal;
        }
    }

    TvShowContainer TvEpisodeContainer::getTvShow(bool* p_found) {
        return TvShowContainer(show_title, show_date, true, p_found);
    }

    TvSeasonContainer TvEpisodeContainer::getTvSeason(bool* p_found) {
        return TvSeasonContainer(show_title, show_date, season, true, p_found);
    }


    void TvEpisodeContainer::loadPosterImg() {
        poster_img = Global::asset_manager->loadTexture(poster_path);
    }

}