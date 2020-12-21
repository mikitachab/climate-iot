#include <string>
#include <sqlite3.h>


class SQLite {
public:
    SQLite(std::string dbPath);
    ~SQLite();
private:
    sqlite3* db;
};
