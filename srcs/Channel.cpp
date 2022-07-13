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

bool        Channel::addClient(Client *toAdd){
    if (toAdd == searchBanned(toAdd->getUname()))
    {
        clientLogMssg("client can't be added (banned)");
        return (false);
    }
    return _clients.insert(std::make_pair(toAdd->getUname(), toAdd)).second;
}

bool    Channel::addToBanList(Client *toBan)
{
    return _clientsban.insert(std::make_pair(toBan->getUname(), toBan)).second;
}

bool    Channel::kickFromChannel(Client *toKick)
{
    clientlist::iterator it = _clients.find(toKick->getUname());
    if (it == _clients.end())
        return (0);
    else
    {
        _clients.erase(it);
        return (true);
    }
}

Client      *Channel::searchClient(std::string username){
    clientlist::iterator it = _clients.find(username);
    if (it == _clients.end())
        return (NULL);
    return (it->second);
}

Client      *Channel::searchBanned(std::string username){
    clientlist::iterator it = _clientsban.find(username);
    if (it == _clientsban.end())
        return (NULL);
    return (it->second);
}

std::string Channel::getName(){
    return (_name);
}

const Channel::clientlist Channel::getBannedClients() const
{
    return _clientsban;
}
