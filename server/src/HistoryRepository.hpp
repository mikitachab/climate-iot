#pragma once
#include <string>
#include <iostream>

#include "SQLite.hpp"

struct CreateHistoryTablesQuery
{
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        return 0;
    }
    std::string sql()
    {
        std::string sqlStr = "CREATE TABLE IF NOT EXISTS device("
                             "id INT PRIMARY KEY NOT NULL,"
                             "name TEXT NOT NULL);";
        sqlStr += "CREATE TABLE IF NOT EXISTS history("
                  "id INT PRIMARY KEY NOT NULL,"
                  "device_id INEGER NOT NULL,"
                  "temperature INEGER NOT NULL,"
                  "datetime DATETIME NOT NULL,"
                  "FOREIGN KEY(device_id) REFERENCES device(id));";
        return sqlStr;
    }

    SQLite::Query query()
    {
        return SQLite::Query{sql(), callback};
    }
};

class HistoryRepository
{
public:
    HistoryRepository(std::shared_ptr<SQLite> db) : db(db) {}
    int init()
    {
        CreateHistoryTablesQuery chtq;
        return db->execute(chtq.query());
    }
    std::string error()
    {
        if (db->errorMsg.size() == 0)
        {
            return "unknown";
        }
        return db->errorMsg;
    }

private:
    std::shared_ptr<SQLite> db;
};
