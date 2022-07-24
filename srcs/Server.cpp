/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:06:57 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/24 17:44:10 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

 Server::Server()
{
	// init_socket(8080);
	// Client &tmp = *(new Client(this, "Server_Machine_Admin"));
	// tmp.becomeOperator();
	// addClient(tmp, 0);
	// _servercommands.insert(std::make_pair("NICK", new Nick(this)));
	// _servercommands.insert(std::make_pair("OPER", new Oper(this)));
	// _servercommands.insert(std::make_pair("HELP", new Help(this)));
	// _servercommands.insert(std::make_pair("JOIN", new Join(this)));
	// _servercommands.insert(std::make_pair("LIST", new List(this)));
	// _servercommands.insert(std::make_pair("USER", new Usercmd(this)));
	// _servercommands.insert(std::make_pair("QUIT", new Quit(this)));
	// _servercommands.insert(std::make_pair("PING", new Ping(this)));
	// _servercommands.insert(std::make_pair("PRIVMSG", new PrivMsg(this)));
    // /* TO DO
    // */
    // _opcommands.insert(std::make_pair("BAN", new ChannelBan(this)));
	// _opcommands.insert(std::make_pair("KICK", new Kick(this)));
    // /* TO DO
    // KICK <channel> <client> :[<message>] (does not ban just kick)
    // KILL <client> <comment>
    // DIE (command to shutdown server)
    // */
}

Server::Server(int port, std::string pwd)
:
_server_pwd(pwd)
{
	init_socket(port);


	/**************************************************************************/

	_clientSockets.push_back(this->createPollfd(_entrySocket));
	Client &tmp = *(new Client(this, "Server_Machine_Admin", "SM_Admin", "SM_Admin"));
	tmp.changeName(std::string("ServerAdmin"));
	tmp.becomeOperator();
	addClient(tmp, 0);


	/**************************************************************************/

	_servercommands.insert(std::make_pair("NICK", new Nick(this)));
	_servercommands.insert(std::make_pair("OPER", new Oper(this)));
	_servercommands.insert(std::make_pair("HELP", new Help(this)));
	_servercommands.insert(std::make_pair("JOIN", new Join(this)));
	_servercommands.insert(std::make_pair("LIST", new List(this)));
	_servercommands.insert(std::make_pair("USER", new Usercmd(this)));
	_servercommands.insert(std::make_pair("QUIT", new Quit(this)));
	_servercommands.insert(std::make_pair("PING", new Ping(this)));
	_servercommands.insert(std::make_pair("PRIVMSG", new PrivMsg(this)));
    /* TO DO
	None for now
    */
    _opcommands.insert(std::make_pair("BAN", new ChannelBan(this)));
	_opcommands.insert(std::make_pair("KICK", new Kick(this)));
    /* TO DO
    KILL <client> <comment>
    DIE (command to shutdown server)
    */
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
 
Server::~Server()
{
    serverLogMssg("Server shut down");
}

/*
** --------------------------------- PUBLIC METHODS ----------------------------
*/


//	sending message (*mssg* std::string) to a specific (*sendTo* client), adding prefixed server name and \r\n
void	Server::sendToClient(Client const &sendTo, std::string mssg)
{
	int size;

	size =	mssg.size() 
			+ std::string(std::string(SERVER_NAME) 
			+ std::string(" ")).size() 
			+ 3;

	send(sendTo.getFd(),
		(void *)std::string
			(
				std::string(SERVER_NAME) 
				+ std::string(" ") 
				+ mssg 
				+ std::string("\r\n")
			).c_str(),
		 size,
		 0);

	
	serverLogMssg(std::string
			(
				std::string("message sent to <")
				+ sendTo.getNname()
				+ std::string("> :|")
				+ std::string(SERVER_NAME) 
				+ std::string(" ") 
				+ mssg 
				+ std::string("|\r\n")
			));
}

//	main server routine, accepting client  and preccesing requests
void	Server::routine()
{
	while (1)
	{
		this->pollRoutine();
	}
}

//	adding client, using recv -> parsing user info -> adding new user | sending an error mssg
void	Server::addClient(Client const & user, int fd)
{
	_clientSockets.push_back(createPollfd(fd));
	_usersMap.insert(std::make_pair(fd, user));
	serverLogMssg(std::string(std::string("new client <") + user.getNname() + std::string("> added to the client list")));
}

// deleting client
void	Server::deleteClient(std::string uname)
{
	std::map<int, Client>::iterator it = _usersMap.begin();
	std::map<int, Client>::iterator ite = _usersMap.end();
	while (it != ite)
	{
		if (it->second.getUname() == uname)
		{
			std::vector<struct pollfd>::iterator itt = _clientSockets.begin();
			std::vector<struct pollfd>::iterator itte = _clientSockets.end();
			while (itt != itte)
			{
				if (itt->fd == it->first)
					_clientSockets.erase(itt);
				itt++;
			}
			close(it->first);
			_usersMap.erase(it);
			break;
		}
		it++;
	}
}

// adding new channel to existing one's
Server::channelmap::iterator Server::addChannel(Channel &newchan)
{
	return _channels.insert(std::make_pair(newchan.getName(), newchan)).first;
}

// searching channel by *channame*, returning a pointer to the found channel, NULL if not found
Channel *Server::searchChannel(std::string channame)
 {
    channelmap::iterator it = _channels.find(channame);
    if (it == _channels.end())
        return (NULL);
    return (&(it->second));
}

/*
** --------------------------------- PRIVATE METHODS ---------------------------
*/

void Server::init_socket(int port)
{
    _entrySocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    serverLogMssg("socket created");
    
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    _address.sin_port = htons(port);

    if ((bind(_entrySocket, (struct sockaddr *)&_address, sizeof(_address))) != 0)
    {
        serverLogMssg("socket bind failure");
        return ;
    }
    else
        serverLogMssg("socket bind succeded");

    if ((listen(_entrySocket, 1024)) != 0)
    {
        serverLogMssg("listen() failure");
        return ;
    }
    else
        serverLogMssg("server is listening");
    return ;
}

// executin commands typed on the server terminal, serv.admin only
void	Server::executeMachCmds(char * buff)
{
	buff[read(0, buff, 552)] = 0;
	*logStream << "\treceived mssg = " << buff << std::endl;
	std::string tmp(buff);
	if (tmp.compare("exit\n") == 0)
		exit(0);
	else if (tmp.compare("list users\n") == 0)
	{
		*logStream << "LIST OF USERS:" << std::endl;
		for (clientmap::iterator it = _usersMap.begin(); it != _usersMap.end(); it++)
			*logStream << "fd <" << it->first << "> user <" << it->second.getNname() << std::endl;
		*logStream << std::endl;
	}
	else if (tmp.compare("channel list\n") == 0)
	{
		*logStream << "LIST OF CHANNELS:" << std::endl;
		for (channelmap::iterator it = _channels.begin(); it != _channels.end(); it++)
			*logStream << "channel <" << it->first << "> with " << it->second.getClients().size() << " users" << std::endl;
		*logStream << std::endl;

	}
	else if (tmp.compare("create channel\n") == 0)
	{
		std::string newChannelName;
		std::cout << "Type the new channel name, then press ENTER" << std::endl;
		std::cin >> newChannelName;

		addChannel(*(new Channel(this, newChannelName)));
	}
	// .....
	Client &admin = _usersMap.find(0)->second;
	parseClientSent(buff, admin);
}

bool	Server::parseClientSent(char * buff, Client &user) 
{
	std::vector<std::string> msgs = ftirc_split(buff, "\r\n");
	std::vector<std::string>::iterator msgit = msgs.begin();
	size_t pos;
	while (msgit != msgs.end()) {
		pos = msgit->find(' ');
		if (pos == std::string::npos)
			pos = msgit->size();
		if (!user.execute(msgit->substr(0, pos), msgit->substr((pos == msgit->size() ? pos : pos + 1))))
		{
			clientLogMssg(*msgit);
		}
		++msgit;
	}
	if (user.getUname().find("test user") != std::string::npos)
		return (false);
	return (true);
}

// proccesing events if any event occured on a socket
void	Server::proccessEventFd(int i)
{
	char buff[552];
	*logStream << "\t_clientSockets[" << i << "] had a revent, fd = " << _clientSockets[i].fd << std::endl;
	if (_clientSockets[i].fd == 0)
		this->executeMachCmds(buff);
	else if (_clientSockets[i].fd == _entrySocket)
		this->acceptClient();
	else
	{
		clientmap::iterator it =  _usersMap.find(_clientSockets[i].fd);
		int recvRet = recv(_clientSockets[i].fd, (void*)buff, 551,0);
		if (recvRet == 0)
		{
			*logStream << "this client disconected, closing the corresponding socket" << std::endl;
			if (it != _usersMap.end())
				this->deleteClient(it->second.getUname());
		}
		else
		{
			buff[recvRet] = 0;
			*logStream << "\treceived mssg = " << buff << std::endl;
			parseClientSent(buff, it->second);
		}
	}
	_clientSockets[i].revents = 0;
}

// main poll routine, checking and proccessing events if any occurs
void	Server::pollRoutine()
{
	if (poll(&(*(_clientSockets.begin())), _clientSockets.size(), 500) > 0)
	{
		serverLogMssg(" pool detected something:");
		for (unsigned int i = 0; i < _clientSockets.size(); i++)
		{
			if (_clientSockets[i].revents != 0)
				this->proccessEventFd(i);
		}
	}
}

// accepting new client, by verifying if the NickName isn't in use already
void	Server::acceptClient()
{
    socklen_t len = sizeof(_client);
    int	client_fd = -1;
    char buff[552];
	int recvRet = 0;
	
	/**************************************************************************
	if (recvRet > 551 || recvRet < 0)
		buff[0] = 0;
	if (recvRet == -1)
	{
		int ernonb = errno;
		*logStream << "\nERROR: erno is set to:" << ernonb << std::endl << std::endl;
	}
	else
		buff[recvRet] = 0;

	**************************************************************************/

	client_fd = accept(_entrySocket, (struct sockaddr *)&_client, &len);
 	if (client_fd >= 0)
  	{
		recvRet = recv(client_fd, (void*)buff, 551, 0);
		if (recvRet > 551 || recvRet < 0)
			buff[551] = 0;
		else
			buff[recvRet] = 0;

		/**************************************************************************/
		*logStream << "(SERVER): new client try to join, the client message (size= ";
		*logStream << std::string(buff).size() << ") (resvRet= " << recvRet << ")";
		*logStream << std::endl << buff << std::endl;
		/**************************************************************************/

		if (recvRet  != 0 && std::string(buff).size() != 0)
		{
			Client &tmp = *(new Client(this, "test user", client_fd));
			this->sendToClient(tmp, "001 new_user :Welcome to our first IRC server for 42.paris!");
			if (parseClientSent(buff, tmp) == true)
			{
				this->sendToClient(tmp, "001 new_user :Welcome to our first IRC server for 42.paris!");
				this->addClient(tmp, client_fd);
			}
			else
				delete(&tmp);
		}
 	}
	else
		serverLogMssg("accept failure");
}


//	returning serverhash
std::string		&Server::serverhash(std::string &toHash) const {
	return (toHash);
}


// checking if the password to entry is correct
bool			Server::checkOpPass(std::string pass) const {
	if (serverhash(pass) == _passop)
		return (true);
	return (false);
}

bool	Server::checkServerPass(std::string pass) const {
	if (serverhash(pass) == _server_pwd)
		return (true);
	return (false);
}

struct pollfd&	Server::createPollfd(int fd)
{
	struct pollfd *tmp = (struct pollfd*)malloc(sizeof(struct pollfd));
	struct pollfd &tmp1 = *tmp;
	tmp->fd = fd;
	tmp->events = POLLIN;
	tmp->revents = 0;
	return (tmp1);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const Server::commandmap &Server::getServCommands() const {
    return (_servercommands);
}

const Server::commandmap &Server::getOpCommands() const {
    return (_opcommands);
}

const Server::channelmap &Server::getChannels() const {
    return(_channels);
}

const Server::clientmap &Server::getClients() const {
	return(_usersMap);
}

/* ************************************************************************** */