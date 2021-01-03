#pragma once
#include <iostream>
#include <exception>

#include <boost/program_options.hpp>
#include <httplib.h>

namespace po = boost::program_options;

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

struct ShowDeviceHistory : Command
{
    void execute(po::variables_map args)
    {
        std::cout << "show device history" << std::endl;
        const std::string device = args["device"].as<std::string>();
        httplib::Client client("localhost", 9081); // TODO get it from env var or boost::po
        const std::string url = "/device/history/" + device;
        std::cout << url << std::endl;
        auto res = client.Get(url.c_str());
        std::cout << "result body"<< std::endl;
        std::cout << res->body << std::endl;
    }
};
