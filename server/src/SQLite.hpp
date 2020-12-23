#pragma once
#include <string>
#include <sqlite3.h>

class SQLite {
public:
    struct Query {
        std::string sql;
        sqlite3_callback cb;
    };

    SQLite(std::string dbPath);
    ~SQLite();

    bool good;
    std::string errorMsg;
    int execute(Query q);
private:
    sqlite3* db;
};
