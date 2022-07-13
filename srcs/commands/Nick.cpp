#include "commands/Nick.hpp"
#include "Client.hpp"

Nick::Nick() {
}

Nick::Nick(Server *serv) : Command(serv) {

}

std::string Nick::help_msg() const {
    return ("/nick newname (allows you to change your username in the server)");
}

void Nick::execute(std::string line, Client &user) {
    user.changeName(line);
    serverLogMssg("Nick command executed");
}