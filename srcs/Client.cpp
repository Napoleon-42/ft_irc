/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:32 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/15 16:12:03 by lnelson          ###   ########.fr       */
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

Client::Client(): _userName("non-spec")
{
    clientLogMssg(std::string("Client " + _userName + " created"));
    _currentChannel = NULL;
    addBasicCommands();
}

Client::Client(Server *current, std::string uname) :
    _serv(current), _userName(uname), _currentChannel(NULL)
{
    clientLogMssg(std::string("Client " + _userName + " created"));
    addBasicCommands();

}

Client::Client(Server *current, std::string uname, int fd) :
	_serv(current), _userName(uname), _fd(fd), _currentChannel(NULL)
{
	clientLogMssg(std::string("Client" + _userName + " created"));
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

void	    Client::execute(std::string &cmd, std::string &restline) {
    commandmap::iterator cit = _commands.find(cmd);
    if (cit == _commands.end()) {
        clientLogMssg("Command '" + cmd + "' not found in available commands for the client : " + _userName);
        _serv->sendToClient(*this, "Command not found.");
    }
    cit->second->execute(restline, *this);
    clientLogMssg("Executed " + cmd + " command.");
}

Command*    Client::searchCommand(std::string cmd) {
    commandmap::iterator cit = _commands.find(cmd);
    if (cit == _commands.end()) {
        clientLogMssg("Command '" + cmd + "' not found for the client : " + _userName);
        return (NULL);
    }
    return (cit->second);
}
