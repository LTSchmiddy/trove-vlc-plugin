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
        runSimpleSqlString("DROP TABLE IF EXISTS movie;");
        runSimpleSqlString("CREATE TABLE movie ("
                           "source VARCHAR,"
                           "path VARCHAR,"
                           "title VARCHAR,"
                           "date INTEGER,"
                           "desc VARCHAR,"
                           "poster_path VARCHAR,"
                           "PRIMARY KEY (source, path)"
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

    int Database::simpleRunStatement(sqlite3_stmt* stmt) {
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
        sqlite3_stmt* stmt = simpleStatementFromString("SELECT source, path, title, date, desc, poster_path FROM movie ORDER BY title;");
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json movie_entry;
            std::string source_name = (char*)sqlite3_column_text(stmt,0);
            std::string path = (char*)sqlite3_column_text(stmt, 1);

            auto source = Global::media_sources->getSourceMap()->at(source_name);
            movie_entry["location"] = source->getUriPrefix() + "/" + path;

            movie_entry["title"] = (char*)sqlite3_column_text(stmt, 2);
            movie_entry["date"] = (char*)sqlite3_column_text(stmt, 3);
            movie_entry["desc"] = (char*)sqlite3_column_text(stmt, 4);
            movie_entry["poster_path"] = fs::absolute((char*)sqlite3_column_text(stmt, 5));
            movies_json.push_back(movie_entry);
        }

        retVal["movies"] = movies_json;
        return retVal;
    }

    void Database::exportToJson(std::string out_path) {
        json exported = dumpToJson();

        std::ofstream out_file(out_path);
        out_file << exported.dump(4);
        out_file.close();
    }
}