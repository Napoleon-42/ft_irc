#include "commands/Join.hpp"
#include "Server.hpp"

Join::Join() {
}

Join::Join(Server *serv) : Command(serv) {

}

std::string Join::help_msg() const {
    return ("/join channel (allows you to join this channel, and creates it if it does not exist yet)");
}

void Join::execute(std::string line, Client &user) {
    std::string channame = line; //To parse
    Channel toadd = Channel(_serv, channame);
    Server::channelmap::iterator it = _serv->addChannel(toadd);
    if (it->second.addClient(&user))
        user.changeChannel(&(it->second));
    serverLogMssg("a user has changed channel");
}