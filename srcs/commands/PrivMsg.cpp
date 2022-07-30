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
    size_t pos = line.find(":");
    std::string target;
    if (*(line.begin()) == '#')
    {
        target = line.substr(1, pos);
        Channel *chan = _serv->searchChannel(target);
        if (chan)
        {
            for (Channel::clientlist::const_iterator it = chan->getClients().begin(); it != chan->getClients().end(); ++it)
            {
                _serv->sendToClient(*(it->second), "PRIVMSG " + line);
            }
        }
    }
    target = line.substr(0, pos);
    Client *tar = _serv->searchClient(target);
    if (!tar) {
        _serv->sendToClient(user, "There is no user with this name on this server.");
        return ;
    }
    _serv->sendToClient(*tar, "PRIVMSG " + user.getUname() + " :" + line.substr(pos));
}