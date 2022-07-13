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
    bool blank = true;
    for (size_t i = 0; i < line.size(); i++)
        if (!isspace(line[i]))
            blank = false;
    if (blank) { //list user on server
        Server::clientmap::const_iterator it = _serv->getClients().begin();
        Server::clientmap::const_iterator ite = _serv->getClients().end();
        while (it != ite) {
            std::cout << it->second.getUname() << std::endl;
        }
    } else { //list users on channel
        Channel::clientlist::const_iterator it = user.getChannel()->getClients().begin();
        Channel::clientlist::const_iterator ite = user.getChannel()->getClients().end();
        while (it != ite) {
            std::cout << it->second->getUname() << std::endl;
        }
    }
}