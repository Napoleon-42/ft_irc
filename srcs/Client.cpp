#include "Client.hpp"

void        Client::addBasicCommands() {
    Server::commandmap::const_iterator it = _serv->getServCommands().begin();
    Server::commandmap::const_iterator ite = _serv->getServCommands().end();
    while (it != ite) {
        commands.insert(std::make_pair(it->first, it->second));
        ++it;
    }
}

void        Client::addOpCommands() {
    Server::commandmap::const_iterator it = _serv->getOpCommands().begin();
    Server::commandmap::const_iterator ite = _serv->getOpCommands().end();
    while (it != ite) {
        commands.insert(std::make_pair(it->first, it->second));
        ++it;
    }
}

Client::Client(): _userName("non-spec")
{
    clientLogMssg(std::string("Client " + _userName + " created"));
    _currentChannel = NULL;
    addBasicCommands();
}

Client::Client(Server *current, std::string uname, std::string hname, std::string sname) :
    _userName(uname), _hostname(hname), _servername(sname)
{
    _serv = current;
    clientLogMssg(std::string("Client " + _userName + " created"));
    _currentChannel = NULL;
    addBasicCommands();

}

Client::~Client()
{
    clientLogMssg(" Client " + _userName + " destroyed");
    
}

void	    Client::becomeOperator() {
    this->addOpCommands();
}

void	    Client::execute(std::string &command, std::string &restline) {
    commandmap::iterator cit = commands.find(command);
    if (cit == commands.end())
        std::cout << "Command '" << command <<"' not found in available commands for the client : " << _userName << std::endl;
    cit->second->execute(restline, *this);
}

Command*    Client::searchCommand(std::string cmd) {
    commandmap::iterator cit = commands.find(cmd);
    if (cit == commands.end()) {
        std::cout << "Command '" << cmd <<"' not found in available commands for the client : " << _userName << std::endl;
        return (NULL);
    }
    return (cit->second);
}
