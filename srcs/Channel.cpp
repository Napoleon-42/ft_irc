/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:40 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/04 01:29:15 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){
    _serv = NULL;
}

Channel::Channel(Server *current, std::string cname)
    : _name(cname), _topic("")
{
    _serv = current;
}

Channel::Channel(const Channel &other)
    : _name(other._name), _params(other._params), _clients(other._clients), _clientsban(other._clientsban), _topic("")
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


void	Channel::changeTopic(std::string newTopic)
{
	_topic = newTopic;
}

Client      *Channel::searchClient(std::string nickname){
    clientlist::iterator it = _clients.find(nickname);
    if (it == _clients.end())
        return (NULL);
    return (it->second);
}

Client      *Channel::searchBanned(std::string nickname){
    clientlist::iterator it = _clientsban.find(nickname);
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
