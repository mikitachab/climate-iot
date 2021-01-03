#include "Controller.hpp"

void Controller::registerCommand(std::string cmdName, std::shared_ptr<Command> cmd)
{
    if (!commandMap.count(cmdName))
    {
        commandMap[cmdName] = cmd;
    }
}

void Controller::dispatch(po::variables_map vm)
{
    std::string cmdName = getCommandName(vm);
    auto cmd = commandMap.at(cmdName);
    cmd->execute(vm);
}

std::string Controller::getCommandName(po::variables_map args)
{
    for (const auto &cmd : commandMap)
    {
        if (args.count(cmd.first))
        {
            return cmd.first;
        }
    }
    throw BadUsage();
}
