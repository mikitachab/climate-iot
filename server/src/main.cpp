// #include <sw/redis++/redis++.h>
#include "ClimateEndpoint.hpp"
#include "SQLite.hpp"


struct CreateHistoryTablesQuery {
  static int callback(void *obj, int argc, char **argv, char **azColName) {
    CreateHistoryTablesQuery* const art = static_cast<CreateHistoryTablesQuery*>(obj);
    std::cout << "table created" << std::endl;
    return 0;
  }
  std::string sql(){
    std::string sqlStr = "CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL);";
    return sqlStr;
  }

  SQLite::Query query() {
    return SQLite::Query{sql(), callback};
  }
};

int main() {
    // Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    // ClimateEndpoint ce(addr);
    // ce.init();
    // ce.start();
    SQLite db("test.db");
    CreateHistoryTablesQuery chtq;
    int err;
    err = db.execute(chtq.query());
    if (err) {
        std::cout << "error: " << db.errorMsg << std::endl;
    }
};
