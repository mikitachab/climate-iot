#pragma once
#include <string>
#include <sqlite3.h>

class SQLite
{
public:
    struct Query
    {
        std::string sql;
        sqlite3_callback cb;
        void *queryObj;
    };

    SQLite(std::string dbPath);
    ~SQLite();

    bool good;
    std::string errorMsg;
    int execute(SQLite::Query q);

private:
    sqlite3 *db;
};
