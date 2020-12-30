#include "memory"

// #include <sw/redis++/redis++.h>
#include "ClimateEndpoint.hpp"
#include "SQLite.hpp"
#include "HistoryRepository.hpp"

int main()
{
    auto db = std::make_shared<SQLite>("test.db");
    HistoryRepository repo(db);
    int error = repo.init();
    if (error)
    {
        std::cout << "error " << repo.error() << std::endl;
        return 1;
    }

    Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    ClimateEndpoint ce(addr, repo);
    ce.init();
    ce.start();

    // error = repo.addDevice("device-0");
    // if (error)
    // {
    //     std::cout << "error " << repo.error() << std::endl;
    //     return 1;
    // }
    // auto devs = repo.getAllDevices();
    // for (const auto &d : devs)
    // {
    //     std::cout << d.id << " " << d.name << std::endl;
    // }
    // error = repo.addHistoryRecord(1, 24.4f, "2020-01-15 10:33:11");
    // if (error)
    // {
    //     std::cout << "error " << repo.error() << std::endl;
    //     return 1;
    // }
    // auto history = repo.getDeviceHistoryByDeviceName("device-0");
    // for (const auto &h : history.data)
    // {
    //     std::cout << h.datetime << " " << h.temperature << std::endl;
    // }
    // std::cout << "ok" << std::endl;
    return 0;
};
