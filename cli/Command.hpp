#pragma once
#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

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

struct ShowDeviceHistory : Command
{
    void execute(po::variables_map args)
    {
        std::cout << "show device history" << std::endl;
    }
};
