#pragma once
#include <iostream>
#include <exception>

#include <boost/program_options.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "ClimateClient/ClimateClient.hpp"

namespace po = boost::program_options;
using json = nlohmann::json;

class BadUsage : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Bad Usage";
    }
};

struct Command
{
    virtual void execute(po::variables_map) = 0;
};

struct ShowDeviceState : Command
{
    void execute(po::variables_map args)
    {
        std::cout << "show state" << std::endl;
    }
};

struct SetDeviceDisplay : Command
{
    void execute(po::variables_map args)
    {
        std::cout << "set device display" << std::endl;
    }
};

class HistoryPrinter
{
public:
    HistoryPrinter(json history) : history(history) {}
    void print()
    {
        for (auto row : history)
        {
            std::cout << row["datetime"] << " " << row["temperature"] << " C" << std::endl;
        }
    }

private:
    json history;
};

struct ShowDeviceHistory : Command
{
    void execute(po::variables_map args)
    {
        ClimateClient c;
        std::cout << "show device history" << std::endl;
        const std::string device = args["device"].as<std::string>();
        json history = c.getDeviceHistoryJson(device);
        if (history.find("message") != history.end())
        {
            std::cout << history["message"] << std::endl;
        }
        else
        {
            HistoryPrinter hp(history);
            hp.print();
        }
        // std::cout << history << std::endl;
    }
};
