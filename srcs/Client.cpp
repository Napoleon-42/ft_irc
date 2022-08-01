/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:32 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/31 13:06:25 by lnelson          ###   ########.fr       */
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
    _serv(current), _username(uname), _currentChannel(NULL), _pwdPass(false), _nick(false), _user(false)
{
    clientLogMssg(std::string("Client " + _username + " created"));
    addBasicCommands();

}

Client::Client(Server *current, std::string uname, int fd) :
	_serv(current), _username(uname), _fd(fd), _currentChannel(NULL), _pwdPass(false), _nick(false), _user(false)
{
	clientLogMssg(std::string("Client " + _username + " created"));
	addBasicCommands();
}

Client::Client(Server *current, std::string uname, std::string hname, std::string sname) :
    _username(uname), _hostname(hname), _servername(sname), _pwdPass(false), _nick(false), _user(false)
{
    _serv = current;
    clientLogMssg(std::string("Client " + _username + " created"));
    _currentChannel = NULL;
    addBasicCommands();

}

Client::Client(Client const & var)
{
	_serv = var.getServer();
	_username = var.getUname();
	_nickname = var.getNname();
	_hostname = var.getHname();
	_servername = var.getSname();
	_realname = var.getRname();
	_fd = var.getFd();
	_currentChannel = var.getChannel();
	_pwdPass = var.getPassStatus();
	_nick = var.getNickstatus();
	_user = var.getUserStatus();
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
        _serv->sendToClient(*this, "421 " + _nickname + " " + cmd + " :Unknown command");
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

std::string Client::getPrefix() const {
	std::string prefix = nickname;
	if (_hostname.length())
	{
		if (_username.length())
			prefix += "!" + username;
		prefix += "@" + _hostname;
	}
	return prefix;
}