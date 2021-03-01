#include "memory"

// #include <sw/redis++/redis++.h>
#include <boost/program_options.hpp>

#include "ClimateHistoryEndpoint.hpp"
#include "SQLite.hpp"
#include "HistoryRepository.hpp"

namespace po = boost::program_options;
po::variables_map parseArgs(int argc, char *argv[], po::options_description od);

int main(int ac, char *av[])
{
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")
            ("port,d", po::value<size_t>()->default_value(9081))
            ("db", po::value<std::string>()->default_value("test.db"));

    auto args = parseArgs(ac, av, desc);


    auto db = std::make_shared<SQLite>(args["db"].as<std::string>());
    HistoryRepository repo(db);
    int error = repo.init();
    if (error)
    {
        std::cout << "error " << repo.error() << std::endl;
        return 1;
    }

    Address addr(Pistache::Ipv4::any(), Pistache::Port(args["port"].as<size_t>()));
    ClimateHistoryEndpoint ce(addr, repo);
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


po::variables_map parseArgs(int argc, char *argv[], po::options_description od)
{
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, od), vm);
    po::notify(vm);
    return vm;
}