#include "commands/Nick.hpp"

Nick::Nick() {
}

Nick::Nick(Client &user) : Command(user) {

}

std::string Nick::help_msg() const {
    return ("/nick newname (allows you to change your username in the server)");
}

void Nick::execute(std::string line) {
    _user->changeName(line);
}