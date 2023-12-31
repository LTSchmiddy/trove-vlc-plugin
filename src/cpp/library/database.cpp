#include <fstream>

#include <plog/Log.h>

#include "Database.h"
#include "settings/settings_globals.h"
#include "media_source/source_globals.h"
#include "assets/asset_globals.h"
// #include "media_source/source_types/FileSource.h"

namespace Library {
    Database::Database() {
        int db_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX |  SQLITE_OPEN_CREATE;

        int startup_result = sqlite3_open_v2(Global::asset_manager->getDataPath(Global::settings.library.path).string().c_str(), &conn, db_flags, NULL);

        if (startup_result != SQLITE_OK) {
            PLOGE << "Error on database startup: " << sqlite3_errmsg(conn);
            return;
        }

        // Count the number of tables in the database:
        sqlite3_stmt* table_count_stmt = simpleStatementFromString("SELECT count(*) FROM sqlite_master WHERE type = 'table';");
        sqlite3_step(table_count_stmt);
        int table_count = sqlite3_column_int(table_count_stmt, 0);
        sqlite3_finalize(table_count_stmt);
        
        // Reconstruct the database if requested:
        if (Global::settings.library.reset_on_load || table_count == 0){
            resetDb();
        }
    }

    Database::~Database() { 
        sqlite3_close_v2(conn);
    }

    void Database::resetDb() {
        PLOGD << "Resetting DB";
        // Movie Stuff
        runSimpleSqlString("DROP TABLE IF EXISTS movie;");
        runSimpleSqlString("CREATE TABLE movie ("
                           "source VARCHAR,"
                           "path VARCHAR,"
                           "title VARCHAR,"
                           "date VARCHAR,"
                           "desc VARCHAR,"
                           "poster_path VARCHAR,"
                           "PRIMARY KEY (source, path)"
                           ");");

        // TV Stuff
        runSimpleSqlString("DROP TABLE IF EXISTS tv_show;");
        runSimpleSqlString("CREATE TABLE tv_show ("
                           "title VARCHAR,"
                           "date INTEGER,"
                           "seasons INTEGER,"
                           "desc VARCHAR,"
                           "poster_path VARCHAR,"
                           "PRIMARY KEY (title, date)"
                           ");");

        runSimpleSqlString("DROP TABLE IF EXISTS tv_season;");
        runSimpleSqlString("CREATE TABLE tv_season ("
                           "show_title VARCHAR,"
                           "show_date VARCHAR,"
                           "season INTEGER,"
                           "title VARCHAR,"
                           "desc VARCHAR,"
                           "poster_path VARCHAR,"
                           "PRIMARY KEY (show_title, show_date, season),"
                           "FOREIGN KEY(show_title, show_date) REFERENCES tv_show(title, date)"
                           ");");

        runSimpleSqlString("DROP TABLE IF EXISTS tv_episode;");
        runSimpleSqlString("CREATE TABLE tv_episode ("
                           "source VARCHAR,"
                           "path VARCHAR,"
                           "title VARCHAR,"
                           "show_title VARCHAR,"
                           "show_date VARCHAR,"
                           "season INTEGER,"
                           "episode INTEGER,"
                           "air_date VARCHAR,"
                           "desc VARCHAR,"
                           "poster_path VARCHAR,"
                           "PRIMARY KEY (source, path),"
                           "FOREIGN KEY(show_title, show_date) REFERENCES tv_show(title, date),"
                           "FOREIGN KEY(show_title, show_date, season) REFERENCES tv_season(show_title, show_date, season)"
                           ");");

        // We'll use this to prevent searching for a show using the same search term more than once,
        // which could happen a lot if we're getting the search term from a folder name.
        runSimpleSqlString("DROP TABLE IF EXISTS tv_show_search_text;");
        runSimpleSqlString("CREATE TABLE tv_show_search_text ("
                           "search_text VARCHAR,"
                           "show_title VARCHAR,"
                           "show_date VARCHAR,"
                           "scraper_vars JSON,"
                           "PRIMARY KEY (search_text),"
                           "FOREIGN KEY(show_title, show_date) REFERENCES tv_show(title, date)"
                           ");");

    }
    // Error Logging:
    void Database::logSqlCompilationError(int prepare_result, std::string sql){
        PLOGE << std::format("Error on compilation of SQL code.\nResult Code: {}\nSQL Statement: {}\nError: {}\n\n", prepare_result,
                                    sql, sqlite3_errmsg(conn));
    }

    void Database::logSqlExecutionError(int step_result, sqlite3_stmt* stmt) {
        PLOGE << std::format("Error on execution of SQL code.\nResult Code: {}\nSQL Statement: {}\nError: {}\n\n", step_result,
                                    sqlite3_sql(stmt), sqlite3_errmsg(conn));
    }

        
    sqlite3* Database::getConn() { return conn; }

    sqlite3_stmt* Database::simpleStatementFromString(std::string sql) {
        sqlite3_stmt* stmt = NULL;

        int prepare_result = sqlite3_prepare_v2(conn, sql.c_str(), sql.length(), &stmt, NULL);

        if (prepare_result != SQLITE_OK) {
            logSqlCompilationError(prepare_result, sql);
        }

        return stmt;
    }

    bool Database::simpleRunStatement(sqlite3_stmt* stmt) {
        int step_result = sqlite3_step(stmt);
        if (!(step_result == SQLITE_DONE || step_result == SQLITE_ROW)) {
            logSqlExecutionError(step_result, stmt);
            return false;
        }

        return true;
    }

    bool Database::runSimpleSqlString(std::string sql) {
        // Create the statement:
        sqlite3_stmt* stmt = simpleStatementFromString(sql);

        if (stmt == NULL) {
            return false;
        }

        // Run the statement:
        bool retVal = simpleRunStatement(stmt);

        sqlite3_finalize(stmt);
        return retVal;
    }

    sqlite3_stmt* Database::returnSimpleSqlString(std::string sql) {
        // Create the statement:
        sqlite3_stmt* stmt = simpleStatementFromString(sql);

        if (stmt == NULL) {
            return NULL;
        }
        // Run the statement:
        simpleRunStatement(stmt);
        return stmt;
    }

    // Where the magic happens: Export the library to something that the Trove SD module in VLC can read from:
    json Database::dumpToJson() {
        json retVal;

        // Load movies:
        json movies_json = json::array();
        sqlite3_stmt* movie_stmt = simpleStatementFromString("SELECT source, path, title, date, desc, poster_path FROM movie ORDER BY title;");
        while (sqlite3_step(movie_stmt) == SQLITE_ROW) {
            json movie_entry;
            std::string source_name = (char*)sqlite3_column_text(movie_stmt,0);
            std::string path = (char*)sqlite3_column_text(movie_stmt, 1);

            auto source = Global::media_sources->getSourceMap()->at(source_name);
            movie_entry["location"] = source->getUriPrefix() + "/" + path;

            movie_entry["title"] = (char*)sqlite3_column_text(movie_stmt, 2);
            movie_entry["date"] = (char*)sqlite3_column_text(movie_stmt, 3);
            movie_entry["desc"] = (char*)sqlite3_column_text(movie_stmt, 4);
            movie_entry["poster_path"] = fs::absolute((char*)sqlite3_column_text(movie_stmt, 5));
            movies_json.push_back(movie_entry);
        }
        sqlite3_finalize(movie_stmt);
        retVal["movies"] = movies_json;

        // Load TV Shows:
        json tv_shows_json = json::array();
        sqlite3_stmt* tv_show_stmt = simpleStatementFromString("SELECT title, date, poster_path FROM tv_show ORDER BY title;");
        while (sqlite3_step(tv_show_stmt) == SQLITE_ROW) {
            json tv_show_entry;

            std::string show_title = (char*)sqlite3_column_text(tv_show_stmt, 0);
            std::string show_date = (char*)sqlite3_column_text(tv_show_stmt, 1);
            std::string show_poster_path = (char*)sqlite3_column_text(tv_show_stmt, 2);
            
            tv_show_entry["title"] = show_title;
            tv_show_entry["poster_path"] = show_poster_path;

            // Load Seasons:
            sqlite3_stmt* tv_season_stmt = simpleStatementFromString(
                "SELECT show_title, show_date, season, title, poster_path FROM tv_season "
                "WHERE show_title = ?, show_date = ? ORDER BY season;"
            );
            sqlite3_bind_text(tv_season_stmt, 1, show_title.c_str(), show_title.length(), NULL);
            sqlite3_bind_text(tv_season_stmt, 2, show_date.c_str(), show_date.length(), NULL);

            json tv_seasons_json = json::array();
            int current_step = sqlite3_step(tv_season_stmt);
            while (current_step == SQLITE_ROW) {
                json tv_season_entry;

                int season_number = sqlite3_column_int(tv_season_stmt, 2);
                std::string season_title = (char*)sqlite3_column_text(tv_season_stmt, 3);
                std::string season_poster_path = (char*)sqlite3_column_text(tv_season_stmt, 4);

                tv_season_entry["season_number"] = season_number;
                tv_season_entry["season_title"] = season_title;
                tv_season_entry["season_poster_path"] = season_poster_path;

                // Load Episodes:
                sqlite3_stmt* tv_episode_stmt = simpleStatementFromString(
                    "SELECT source, path, title, show_title, show_date, season, episode, air_date, desc, poster_path FROM tv_episode "
                    "WHERE show_title = ?, show_date = ?, season = ? ORDER BY episode;"
                );            
                sqlite3_bind_text(tv_episode_stmt, 1, show_title.c_str(), show_title.length(), NULL);
                sqlite3_bind_text(tv_episode_stmt, 2, show_date.c_str(), show_date.length(), NULL);
                sqlite3_bind_int(tv_episode_stmt, 3, season_number);

                json tv_episodes_json = json::array();
                while (sqlite3_step(tv_episode_stmt) == SQLITE_ROW) {
                    json tv_episode_entry;

                    // Constructing location:
                    std::string source_name = (char*)sqlite3_column_text(tv_episode_stmt,0);
                    std::string path = (char*)sqlite3_column_text(tv_episode_stmt, 1);
                    auto source = Global::media_sources->getSourceMap()->at(source_name);
                    tv_episode_entry["location"] = source->getUriPrefix() + "/" + path;

                    tv_episode_entry["title"] = (char*)sqlite3_column_text(tv_episode_stmt, 2);
                    tv_episode_entry["show_title"] = (char*)sqlite3_column_text(tv_episode_stmt, 3);
                    tv_episode_entry["show_date"] = (char*)sqlite3_column_text(tv_episode_stmt, 4);
                    tv_episode_entry["season"] = sqlite3_column_int(tv_episode_stmt, 5);
                    tv_episode_entry["episode"] = sqlite3_column_int(tv_episode_stmt, 6);
                    tv_episode_entry["air_date"] = (char*)sqlite3_column_text(tv_episode_stmt, 7);
                    tv_episode_entry["desc"] = (char*)sqlite3_column_text(tv_episode_stmt, 8);
                    tv_episode_entry["poster_path"] = (char*)sqlite3_column_text(tv_episode_stmt, 9);

                    tv_episodes_json.push_back(tv_episode_entry);
                }
                sqlite3_finalize(tv_episode_stmt);

                tv_season_entry["episodes"] = tv_episodes_json;
                tv_seasons_json.push_back(tv_season_entry);

                current_step = sqlite3_step(tv_season_stmt);
            }
            sqlite3_finalize(tv_season_stmt);
            // Seasons Loaded.
            tv_show_entry["seasons"] = tv_seasons_json;
            tv_shows_json.push_back(tv_show_entry);
        }
        sqlite3_finalize(tv_show_stmt);
        // Shows Loaded.
        retVal["tv_shows"] = tv_shows_json;
        return retVal;
    }

    void Database::exportToJson(std::string out_path) {
        json exported = dumpToJson();

        std::ofstream out_file(out_path);
        out_file << exported.dump(4);
        out_file.close();
    }
}