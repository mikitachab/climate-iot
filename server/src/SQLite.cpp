#include "SQLite.hpp"

#include <iostream>

SQLite::SQLite(std::string dbPath)
{
    int rc;
    rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc)
    {
        good = false;
        errorMsg = std::string(sqlite3_errmsg(db));
    }
    else
    {
        good = true;
    }
}

SQLite::~SQLite()
{
    sqlite3_close(db);
}

int SQLite::execute(SQLite::Query q)
{
    char *queryErrorMsg = 0;
    int rc;

    rc = sqlite3_exec(db, q.sql.c_str(), q.cb, static_cast<void *>(&q), &queryErrorMsg);

    if (rc != SQLITE_OK && queryErrorMsg)
    {
        errorMsg = std::string(queryErrorMsg);
        sqlite3_free(queryErrorMsg);
    }
    return rc;
}
