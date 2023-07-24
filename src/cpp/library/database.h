#pragma once

#include <string>
#include <sqlite3.h>
#include <ns_abbr/json.h>

namespace Library {
    class Database {
    public:
        Database();
        ~Database();
        
        sqlite3* getConn();

        // Resets/Creates the database:
        void resetDb();
        // Error logging:
        void logSqlCompilationError(int prepare_result, std::string sql);
        void logSqlExecutionError(int step_result, sqlite3_stmt* stmt);

        // Generate and run simple statements:
        sqlite3_stmt* simpleStatementFromString(std::string sql);
        int simpleRunStatement(sqlite3_stmt* stmt);
        bool runSimpleSqlString(std::string sql);
        sqlite3_stmt* returnSimpleSqlString(std::string sql);

        json dumpToJson();
        void exportToJson(std::string out_path);


    private:
        sqlite3* conn;
    };
}