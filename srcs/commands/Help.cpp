#include "commands/Help.hpp"
#include "Server.hpp"

Help::Help() {
}

Help::Help(Server *serv) : Command(serv) {

}

std::string Help::help_msg() const {
    return ("/help command (show usage of command)");
}

void Help::execute(std::string line, Client &user) {
    Command *cmd = user.searchCommand(line);
    if (cmd)
        std::cout << cmd->help_msg() << std::endl;
    serverLogMssg("a user has asked for help on a command");
}