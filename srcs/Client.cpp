#include "Client.hpp"

void        Client::addBasicCommands() {
    std::map<std::string, Command>::iterator it = _serv->getServCommands.begin();
    std::map<std::string, Command>::iterator ite = _serv->getServCommands.end();
    while (it != ite) {
        commands.insert(std::make_pair(it->first, &(it->second)));
        ++it;
    }
}

void        Client::addOpCommands() {
    std::map<std::string, Command>::iterator it = _serv->getOpCommands.begin();
    std::map<std::string, Command>::iterator ite = _serv->getOpCommands.end();
    while (it != ite) {
        commands.insert(std::make_pair(it->first, &(it->second)));
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
    _serv(current), _userName(uname), _hostname(hname), _servername(sname)
{
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
    commands::iterator cit = commands.find(command);
    if (cit == commands.end())
        std::cout << "Command '" << command <<"' not found in available commands for the client : " << _userName << std::endl;
    cit->execute(restline, *this);
}

Command&    Client::searchCommand(std::string cmd) {
    commands::iteraor cit = commands.find(command);
    if (cit == commands.end()) {
        std::cout << "Command '" << command <<"' not found in available commands for the client : " << _userName << std::endl;
        return (NULL);
    }
    return (*cit);
}

void        Client::becomeOperator() {
    this = new Operator(this);
}