#include "commands/Quit.hpp"
#include "Client.hpp"

Quit::Quit() {
}

Quit::Quit(Server *serv) : Command(serv) {

}

std::string Quit::help_msg() const {
    return ("/Quit (allows you to Quit the server)");
}

void Quit::execute(std::string line, Client &user) {
    clientLogMssg(line);
    _serv->deleteClient(user.getNname());
}