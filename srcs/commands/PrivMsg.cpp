#include "commands/PrivMsg.hpp"
#include "Server.hpp"

PrivMsg::PrivMsg() {
}

PrivMsg::PrivMsg(Server *serv) : Command(serv) {

}

std::string PrivMsg::help_msg() const {
    return ("/Privmsg <target> :<message> (allows you to send a private msg to someone.)");
}

void PrivMsg::execute(std::string line, Client &user) {
    Channel *chan = user.getChannel();
    if (!chan)
        return ;
    size_t pos = line.find(":");
    std::string target = line.substr(0, pos);
    Client *tar = chan->searchClient(target);
    if (!tar) {
        _serv->sendToClient(user, "There is no user with this name on this channel.");
        return ;
    }
    _serv->sendToClient(*tar, user.getUname() + line.substr(pos));
}