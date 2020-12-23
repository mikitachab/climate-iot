#include "ClimateEndpoint.hpp"
// #include <sw/redis++/redis++.h>
#include "SQLite.hpp"


int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   std::cout << "table created" << std::endl;
   return 0;
}

int main() {
    // Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    // ClimateEndpoint ce(addr);
    // ce.init();
    // ce.start();
    SQLite db("test.db");
    std::string sql = "CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL);";
    SQLite::Query q{sql, callback};
    int err;
    err = db.execute(q);
    if (err) {
        std::cout << "error: " << db.errorMsg << std::endl;
    }
};
