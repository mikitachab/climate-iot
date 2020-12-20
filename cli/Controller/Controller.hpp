#pragma once

#include <boost/program_options.hpp>

#include <unordered_map>
#include <string>

#include "Command.hpp"

namespace po = boost::program_options;

struct Controller
{
    std::unordered_map<std::string, std::shared_ptr<Command>> commandMap;
    std::string getCommandName(po::variables_map args);
    void registerCommand(std::string cmdName, std::shared_ptr<Command> cmd);
    void dispatch(po::variables_map vm);
};
