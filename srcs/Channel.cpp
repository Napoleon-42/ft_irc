#include "Channel.hpp"

Channel::Channel(){
    _serv = NULL;
}

Channel::Channel(Server *current, std::string cname)
    : _name(cname)
{
    _serv = current;
}

Channel::Channel(const Channel &other)
    : _name(other._name), _params(other._params), _clients(other._clients), _clientsban(other._clientsban)
{
    _serv = other._serv;
}

Channel::~Channel(){

}

Channel&    Channel::operator=(const Channel& other){
    _serv = other._serv;
    _name = other._name;
    _params = other._params;
    _clients = other._clients;
    _clientsban = other._clientsban;
    return (*this);

}

const Channel::clientlist    &Channel::getClients() const {
    return (_clients);
}

bool        Channel::addClient(Client *toAdd){
    //lack Check if ban
    return _clients.insert(std::make_pair(toAdd->getUname(), toAdd)).second;
}

Client      *Channel::searchClient(std::string username){
    clientlist::iterator it = _clients.find(username);
    if (it == _clients.end())
        return (NULL);
    return (it->second);
}

std::string Channel::getName(){
    return (_name);
}
