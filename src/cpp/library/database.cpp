#include <plog/Log.h>

#include "database.h"
#include "settings/settings_globals.h"

namespace Library {
    Database::Database() {
        int db_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX |  SQLITE_OPEN_CREATE;

        int startup_result = sqlite3_open_v2(Global::settings.library.path.c_str(), &conn, db_flags, NULL);

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
                           "year INTEGER,"
                           "desc VARCHAR,"
                           "PRIMARY KEY (source, path)"
                           ");");
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

    // Error Logging:
    void Database::logSqlCompilationError(int prepare_result, std::string sql){
        PLOGE << std::format("Error on compilation of SQL code.\nResult Code: {}\nSQL Statement: {}\nError: {}\n\n", prepare_result,
                                    sql, sqlite3_errmsg(conn));
    }

    void Database::logSqlExecutionError(int step_result, sqlite3_stmt* stmt) {
        PLOGE << std::format("Error on execution of SQL code.\nResult Code: {}\nSQL Statement: {}\nError: {}\n\n", step_result,
                                    sqlite3_sql(stmt), sqlite3_errmsg(conn));
    }

}