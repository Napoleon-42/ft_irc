/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:32 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/23 01:05:10 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void        Client::addBasicCommands() {
    Server::commandmap::const_iterator it = _serv->getServCommands().begin();
    Server::commandmap::const_iterator ite = _serv->getServCommands().end();
    while (it != ite) {
        _commands.insert(std::make_pair(it->first, it->second));
        ++it;
    }
}

void        Client::addOpCommands() {
    Server::commandmap::const_iterator it = _serv->getOpCommands().begin();
    Server::commandmap::const_iterator ite = _serv->getOpCommands().end();
    while (it != ite) {
        _commands.insert(std::make_pair(it->first, it->second));
        ++it;
    }
}

Client::Client(): _username("non-spec")
{
    clientLogMssg(std::string("Client " + _username + " created"));
    _currentChannel = NULL;
    addBasicCommands();
}

Client::Client(Server *current, std::string uname) :
    _serv(current), _username(uname), _currentChannel(NULL)
{
    clientLogMssg(std::string("Client " + _username + " created"));
    addBasicCommands();

}

Client::Client(Server *current, std::string uname, int fd) :
	_serv(current), _username(uname), _fd(fd), _currentChannel(NULL)
{
	clientLogMssg(std::string("Client " + _userName + " created"));
	addBasicCommands();
}

Client::Client(Server *current, std::string uname, std::string hname, std::string sname) :
    _username(uname), _hostname(hname), _servername(sname)
{
    _serv = current;
    clientLogMssg(std::string("Client " + _username + " created"));
    _currentChannel = NULL;
    addBasicCommands();

}

Client::~Client()
{
    clientLogMssg(" Client " + _username + " destroyed");
    
}

void	    Client::becomeOperator() {
    this->addOpCommands();
}

bool	    Client::execute(std::string const &cmd, std::string const &restline) {
    commandmap::iterator cit = _commands.find(cmd);
    if (cit == _commands.end()) {
        clientLogMssg("Command '" + cmd + "' not found in available commands for the client : " + _username);
        _serv->sendToClient(*this, "Command not found.");
        return (false);
    }
    clientLogMssg("Executing " + cmd + " command.");
    cit->second->execute(restline, *this);
    clientLogMssg("Executed " + cmd + " command.");
    return (true);
}

Command*    Client::searchCommand(std::string cmd) {
    commandmap::iterator cit = _commands.find(cmd);
    if (cit == _commands.end()) {
        clientLogMssg("Command '" + cmd + "' not found for the client : " + _username);
        return (NULL);
    }
    return (cit->second);
}
