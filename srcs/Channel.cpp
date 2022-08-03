/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:40 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:41 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    if (toAdd == searchBanned(toAdd->getNname()))
    {
        clientLogMssg("client can't be added (banned)");
        return (false);
    }
    return _clients.insert(std::make_pair(toAdd->getNname(), toAdd)).second;
}

bool    Channel::addToBanList(Client *toBan)
{
    return _clientsban.insert(std::make_pair(toBan->getNname(), toBan)).second;
}

bool    Channel::kickFromChannel(Client *toKick)
{
    clientlist::iterator it = _clients.find(toKick->getNname());
    if (it == _clients.end())
        return (0);
    else
    {
        _clients.erase(it);
        return (true);
    }
}

Client      *Channel::searchClient(std::string nickname){
    clientlist::iterator it = _clients.find(nickname);
    if (it == _clients.end())
        return (NULL);
    return (it->second);
}

void    Channel::changenickClient(std::string nick, std::string oldnick) {
    clientlist::iterator it = _clients.find(oldnick);
    if (it == _clients.end())
        return ;
    _clients.insert(std::make_pair(nick, it->second));
    _clients.erase(it);
    return ;
}

bool      Channel::searchClient(std::string nickname) const{
    clientlist::const_iterator it = _clients.find(nickname);
    if (it == _clients.end())
        return (false);
    return (true);
}

Client      *Channel::searchBanned(std::string nickname){
    clientlist::iterator it = _clientsban.find(nickname);
    if (it == _clientsban.end())
        return (NULL);
    return (it->second);
}

std::string Channel::getName() const{
    return (_name);
}

const Channel::clientlist Channel::getBannedClients() const
{
    return _clientsban;
}
