#include "commands/List.hpp"
#include "Server.hpp"
#include "Channel.hpp"

List::List() {
}

List::List(Server *serv) : Command(serv) {

}

std::string List::help_msg() const {
    return ("/list [channel] (list users on channel or on the server if no chaannel specified)");
}

void List::execute(std::string line, Client &user) {
    (void)user;
    bool blank = true;
    for (size_t i = 0; i < line.size(); i++)
        if (!isspace(line[i]))
            blank = false;
    if (blank) { //list user on server
        Server::clientmap::const_iterator it = _serv->getClients().begin();
        Server::clientmap::const_iterator ite = _serv->getClients().end();
        while (it != ite) {
            clientLogMssg(it->second.getUname());
        }
    } else { //list users on channel
        Channel *chan = _serv->searchChannel(line);
        if (!chan) {
            clientLogMssg("This channel does not exist.");
            return ;
        }
        Channel::clientlist::const_iterator it = chan->getClients().begin();
        Channel::clientlist::const_iterator ite = chan->getClients().end();
        while (it != ite) {
            clientLogMssg(it->second->getUname());
        }
    }
}