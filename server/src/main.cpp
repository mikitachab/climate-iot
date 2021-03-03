#include "memory"

// #include <sw/redis++/redis++.h>
#include <boost/program_options.hpp>

#include "ClimateServer.hpp"
#include "SQLite.hpp"
#include "HistoryRepository.hpp"

namespace po = boost::program_options;
po::variables_map parseArgs(int argc, char *argv[], po::options_description od);

int main(int ac, char *av[])
{
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")
            ("port", po::value<size_t>()->default_value(9082))
            ("db", po::value<std::string>()->default_value("test.db"));

    auto args = parseArgs(ac, av, desc);

    std::cout << args["port"].as<size_t>() << std::endl;


    auto db = std::make_shared<SQLite>(args["db"].as<std::string>());
    HistoryRepository repo(db);
    int error = repo.init();
    if (error)
    {
        std::cout << "error " << repo.error() << std::endl;
        return 1;
    }

    Address addr(Pistache::Ipv4::any(), Pistache::Port(args["port"].as<size_t>()));
    ClimateServer ce(addr, repo);
    ce.init();
    ce.start();

    return 0;
};


po::variables_map parseArgs(int argc, char *argv[], po::options_description od)
{
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, od), vm);
    po::notify(vm);
    return vm;
}