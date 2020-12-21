#include "SQLite.hpp"

SQLite::SQLite(std::string dbPath) {
    sqlite3_open(dbPath.c_str(), &db);
    // TODO check open success
}

SQLite::~SQLite() {
    sqlite3_close(db);
}
