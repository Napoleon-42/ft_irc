#include "commands/Help.hpp"

Help::Help() {
}

Help::Help(Client &user) : Command(user) {

}

std::string Help::help_msg() const {
    return ("/help command (show usage of command)");
}

void Help::execute(std::string line) {
    Command *cmd = _user->searchCommand(line);
    if (cmd)
        std::cout << cmd->help_msg() << std::endl;
}