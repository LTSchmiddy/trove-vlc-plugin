#pragma once

#include <sqlite3.h>

namespace Library {
    class Database {
    public:
        Database();
        ~Database();
        
        sqlite3* getConn();

        // Resets/Creates the database:
        void resetDb();

        // Generate and run simple statements:
        sqlite3_stmt* simpleStatementFromString(std::string sql);
        int simpleRunStatement(sqlite3_stmt* stmt);
        bool runSimpleSqlString(std::string sql);
        sqlite3_stmt* returnSimpleSqlString(std::string sql);

        // Error logging:
        void logSqlCompilationError(int prepare_result, std::string sql);
        void logSqlExecutionError(int step_result, sqlite3_stmt* stmt);
    private:
        sqlite3* conn;
    };
}