#include <iostream>
#include <memory>

#include <boost/program_options.hpp>

#include "Command.hpp"
#include "Controller/Controller.hpp"

namespace po = boost::program_options;

po::variables_map parseArgs(int argc, char *argv[], po::options_description od);

int main(int ac, char *av[])
{
    po::options_description desc("Allowed options");
    try
    {
        desc.add_options()("help,h", "produce help message")
            ("display", "set display text for device")
            ("history", "show story")
            ("state", "show story");

        auto args = parseArgs(ac, av, desc);

        if (args.count("help"))
        {
            std::cout << desc << std::endl;
        }
        Controller controller;
        controller.registerCommand("display", std::make_shared<SetDeviceDisplay>());
        controller.registerCommand("history", std::make_shared<ShowDeviceHistory>());
        controller.registerCommand("state", std::make_shared<ShowDeviceState>());


        controller.dispatch(args);

    }
    catch (std::exception &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

po::variables_map parseArgs(int argc, char *argv[], po::options_description od)
{
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, od), vm);
    po::notify(vm);
    return vm;
}
