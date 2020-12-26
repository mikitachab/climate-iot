#include "memory"

// #include <sw/redis++/redis++.h>
#include "ClimateEndpoint.hpp"
#include "SQLite.hpp"
#include "HistoryRepository.hpp"

int main()
{
    // Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    // ClimateEndpoint ce(addr);
    // ce.init();
    // ce.start();
    auto db = std::make_shared<SQLite>("test.db");
    HistoryRepository repo(db);
    int error = repo.init();
    if (error)
    {
        std::cout << "error " << repo.error() << std::endl;
        return 1;
    }
    std::cout << "ok" << std::endl;
    return 0;
};
