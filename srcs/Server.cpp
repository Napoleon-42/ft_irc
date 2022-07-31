/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:06:57 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/31 21:51:17 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

 Server::Server()
{
}

Server::Server(int port, std::string pwd)
:
_server_pwd(pwd)
{
	init_socket(port);
	_server_pwd = serverhash(pwd);

	_servercommands.insert(std::make_pair("NICK", new Nick(this)));
	_servercommands.insert(std::make_pair("OPER", new Oper(this)));
	_servercommands.insert(std::make_pair("HELP", new Help(this)));
	_servercommands.insert(std::make_pair("JOIN", new Join(this)));
	_servercommands.insert(std::make_pair("LIST", new List(this)));
	_servercommands.insert(std::make_pair("USER", new Usercmd(this)));
	_servercommands.insert(std::make_pair("QUIT", new Quit(this)));
	_servercommands.insert(std::make_pair("PING", new Ping(this)));
	_servercommands.insert(std::make_pair("PRIVMSG", new PrivMsg(this)));
	_servercommands.insert(std::make_pair("PASS", new Pass(this)));

    _opcommands.insert(std::make_pair("BAN", new ChannelBan(this)));
	_opcommands.insert(std::make_pair("KICK", new Kick(this)));
    /* TO DO
    KILL <client> <comment>
    DIE (command to shutdown server)
    */

	/**************************************************************************/
	_clientSockets.push_back(this->createPollfd(_entrySocket));
	Client tmp(this, "Server_Machine_Admin", "SM_Admin", "SM_Admin");
	tmp.changeName(std::string("ServerAdmin"));
	tmp.becomeOperator();
	addClient(tmp, 0);
	/**************************************************************************/
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


//	main server routine, accepting client  and preccesing requests
void							Server::routine()
{
	while (1)
	{
		this->pollRoutine();
	}
}


//	sending message (*mssg* std::string) to a specific (*sendTo* client), adding prefixed server name and \r\n
void							Server::sendToClient(Client &sendTo, std::string mssg)
{
	int size;

	size =	mssg.size() + 
			std::string(std::string(SERVER_NAME) + 
			" ").size() + 
			3;

	send(sendTo.getFd(),
		(void *)std::string
			(
				std::string(SERVER_NAME) + 
				" " + 
				mssg + 
				"\r\n"
			).c_str(),
		 size,
		 0);

	
	serverLogMssg(std::string
			(
				"message sent to <"
				+ sendTo.getNname()
				+ "> :|"
				+ SERVER_NAME
				+ " "
				+ mssg 
				+ "|\r\n"
			));
}





// adding new channel to existing one's
Server::channelmap::iterator	Server::addChannel(Channel &newchan)
{
	return _channels.insert(std::make_pair(newchan.getName(), newchan)).first;
}





//	adding client, using recv -> parsing user info -> adding new user | sending an error mssg
void	Server::addClient(Client user, int fd)
{
	_clientSockets.push_back(createPollfd(fd));
	_usersMap.insert(std::make_pair(fd, user));
	serverLogMssg(std::string("new client <" + user.getNname() + " > added to the client list"));
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
			deleteFdfPoll(it->first);
			_usersMap.erase(it);
			break;
		}
		it++;
	}
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





// search a client int the corresponding map, by his fd
Client *	Server::searchClient(int fd, clientmap * map) 
{
	clientmap::iterator it = map->find(fd);
	if (it == map->end())
		throw UserNotFound();
	return (&(it->second));
}



Client	*	Server::searchClient(std::string nickName) 
{
	std::map<int, Client>::iterator it = _usersMap.begin();
	std::map<int, Client>::iterator ite = _usersMap.end();
	while (it != ite)
	{
		if (it->second.getNname() == nickName)
			return (&(it->second));
		it++;
	}
	return (NULL);
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
	struct sockaddr_in _address;

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
		try
		{
			Client * tmp = searchClient(_clientSockets[i].fd, &_pendingClients);
			serverLogMssg("entering ProccessPendingClient");
			proccessPendingClient(tmp);
		}
		catch(Server::UserNotFound &nte)
		{
			try
			{
				Client * tmp = searchClient(_clientSockets[i].fd, &_usersMap);
				serverLogMssg("entering ProccessRegisteredClient");
				proccessRegisteredClient(tmp);
			}
			catch(Server::UserNotFound &nte)
			{
				serverLogMssg("deleting a Fd from poll");
				deleteFdfPoll(_clientSockets[i].fd);
			}
		}
		
	}
	_clientSockets[i].revents = 0;
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
	} else {
		Client &admin = _usersMap.find(0)->second;
		parseClientSent(buff, admin);
	}
}


// accepting new client, by verifying if the NickName isn't in use already
void	Server::acceptClient()
{
	struct sockaddr_in _client;

    socklen_t len = sizeof(_client);
    int	client_fd = -1;
	
	client_fd = accept(_entrySocket, (struct sockaddr *)&_client, &len);
 	if (client_fd >= 0)
		addPendingClient(Client(this, "pending_user", client_fd), client_fd);
	else
		serverLogMssg("accept failure");
}


void	Server::proccessPendingClient(Client * pendingClient)
{
	char buff[553];
	int rcvRet;

	rcvRet = recv(pendingClient->getFd(), (void*) buff, 552, 0);
	if (rcvRet > 0)
	{
		if (rcvRet > 552)
			buff[552] = 0;
		else
			buff[rcvRet] = 0;
		
								*logStream << "\treceived mssg = " << buff << std::endl;
								*logStream << "PROCESSING PENDING CLIENT:" << std::endl;

		std::vector<std::string> msgs = ftirc_split(buff, "\r\n");

		std::vector<std::string>::iterator msgsit = msgs.begin();
		std::vector<std::string>::iterator msgsite = msgs.end();
		
		while( msgsit != msgsite)
		{
			size_t first_space = msgsit->find(' ');
			std::string cmdName = msgsit->substr(0, first_space);
			std::string cmdArgs = msgsit->substr((first_space == msgsit->size() ? first_space : first_space + 1));

								*logStream << "str = |" << cmdName << "' '" << cmdArgs << "|" << std::endl;

			if (pendingClient->getPassStatus() == false)
			{
				if (cmdName == "PASS")
				{
					if (!this->checkServerPass(cmdArgs))
						sendToClient(*pendingClient, "464 * :Password incorrect");
					else
						pendingClient->validatePass();
				}
			}

			else
			{
				if (cmdName == "NICK" || cmdName == "USER")
				{
					if (cmdName == "NICK")
					{
						try
						{
							pendingClient->execute(cmdName, cmdArgs);
							pendingClient->validateNick();
						}
						catch(const std::exception& e)
						{
							sendToClient(*pendingClient,
							std::string(
								std::string("433 * ") +
								cmdArgs +
								std::string(" :Nickname is already in use")));
						}
					}
					else
					{
						pendingClient->execute(cmdName, cmdArgs);
						pendingClient->validateUser();
					}
				}
			}

			msgsit++;
		}
	}
	else if (rcvRet == 0)
	{
		*logStream << "One of Pending client's disconected, closing the corresponding socket" << std::endl;
		deleteFdfPoll(pendingClient->getFd());
		_pendingClients.erase(pendingClient->getFd());
	}	

	if (pendingClient->getNickstatus() && pendingClient->getUserStatus() && pendingClient->getPassStatus())
	{
		addClient(Client(*pendingClient), pendingClient->getFd());
		this->sendToClient(*pendingClient,
		std::string("001 " +
			pendingClient->getNname() +
			std::string(" :Welcome to our first IRC server for 42.paris!")));
		_pendingClients.erase(pendingClient->getFd());
	}
}


void	Server::proccessRegisteredClient(Client * client)
{
	char buff[553];
	int rcvRet;

	rcvRet = recv(client->getFd(), (void*) buff, 552, 0);
	if (rcvRet > 0)
	{
		if (rcvRet > 552)
			buff[552] = 0;
		else
			buff[rcvRet] = 0;

		*logStream << "\treceived mssg = " << buff << std::endl;

		std::vector<std::string> msgs = ftirc_split(buff, "\r\n");

		std::vector<std::string>::iterator msgsit = msgs.begin();
		std::vector<std::string>::iterator msgsite = msgs.end();
		
		while( msgsit != msgsite)
		{
			size_t first_space = msgsit->find(' ');
			std::string cmdName = msgsit->substr(0, first_space);
			std::string cmdArgs = msgsit->substr((first_space == msgsit->size() ? first_space : first_space + 1));

			try
			{
				client->execute(cmdName, cmdArgs);
			}
			catch (Nick::NameTakenException &nte)
			{
				sendToClient(*client,
				std::string(
					std::string("433 ") +
					client->getNname() +
					" " +
					cmdArgs +
					std::string(" :Nickname is already in use")));
			}
			msgsit++;
		}
		
	}
	else if (rcvRet == 0)
	{
		*logStream << "One of Registered client's disconected, closing the corresponding socket" << std::endl;
		deleteClient(client->getUname());
	}

}






//	add a client in the "pending list", for clients who doesn't send right PASS NICK and USER
void	Server::addPendingClient(Client pendingUser, int fd)
{
	_clientSockets.push_back(createPollfd(fd));
	_pendingClients.insert(std::make_pair(fd, pendingUser));
	serverLogMssg("Added new pending user");
}

 
bool	Server::parseClientSent(char * buff, Client &user) 
{
	std::vector<std::string> msgs = ftirc_split(buff, "\r\n");
	std::vector<std::string>::iterator msgit = msgs.begin();

	size_t pos;
	try {
		while (msgit != msgs.end()) {
			pos = msgit->find(' ');
			if (pos == std::string::npos)
				pos = msgit->size();
			if (!user.execute(msgit->substr(0, pos), msgit->substr((pos == msgit->size() ? pos : pos + 1))))
				clientLogMssg(*msgit);
			++msgit;
		}
	} catch (Nick::NameTakenException &nte) {
		sendToClient(user, nte.what());
		return (false);
	}
	return (true);
}






//	returning serverhash
std::string		&Server::serverhash(std::string &toHash) const {
	std::string ret = "";
	for(std::string::size_type i = 0; i < toHash.size(); ++i) {
    	ret += toHash[i] + 1;
		// si plus compliqué nécessaire : (toHash[i] + i) % 126 + 1;
	}
	//Debugging
	serverLogMssg("Hashed : " + toHash + " into " + ret);
	toHash = ret;
	return (toHash);
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

void			Server::deleteFdfPoll(int fd)
{
	std::vector<struct pollfd>::iterator it = _clientSockets.begin();
	std::vector<struct pollfd>::iterator ite = _clientSockets.end();

	close(fd);
	while (it != ite)
	{
		if (it->fd == fd)
		{
			_clientSockets.erase(it);
			break;
		}
		it++;
	}
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