/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:06:57 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/23 01:04:49 by lnelson          ###   ########.fr       */
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
	Client &tmp = *(new Client(this, "Server_Machine_Admin"));
	tmp.becomeOperator();
	addClient(tmp, 0);
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
void	Server::sendToClient(Client sendTo, std::string mssg)
{
	int size;

	size =	mssg.size() 
			+ std::string(std::string(SERVER_NAME) 
			+ std::string(" ")).size() 
			+ 3;

	send(sendTo.getFd(),
		(void *)std::string(
			std::string(SERVER_NAME) 
			+ std::string(" ") 
			+ mssg 
			+ std::string("\r\n")).c_str(),
		 size,
		 0);
}

//	main server routine, accepting client  and preccesing requests
void	Server::routine()
{
	while (1)
	{
		this->acceptClient();
		this->pollRoutine();
	}
}

//	adding client, using recv -> parsing user info -> adding new user | sending an error mssg
void	Server::addClient(Client const & user, int fd)
{
	struct pollfd *tmp = (struct pollfd*)malloc(sizeof(struct pollfd));
	struct pollfd &tmp2 = *tmp;
	tmp->fd = fd;
	tmp->events = POLLIN;
	tmp->revents = 0;
	_clientSockets.push_back(tmp2);
	_usersMap.insert(std::make_pair(fd, user));
	serverLogMssg("new client added");
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
	*logStream << "\treceived mssg = " << buff;
	std::string tmp(buff);
	if (tmp.compare("exit\n") == 0)
		exit(0);
	Client &admin = _usersMap.find(0)->second;
	parseClientSent(buff, admin);
}

void	Server::parseClientSent(char * buff, Client &user) {
	std::vector<std::string> msgs = ftirc_split(buff, "\r\n");
	std::vector<std::string>::iterator msgit = msgs.begin();
	size_t pos;
	while (msgit != msgs.end()) {
		pos = msgit->find(' ');
		if (pos == std::string::npos)
			pos = msgit->size();
		if (!user.execute(msgit->substr(0, pos), msgit->substr((pos == msgit->size() ? pos : pos + 1))))
			clientLogMssg(*msgit);
		++msgit;
	}
}

// proccesing events if any event occured on a socket
void	Server::proccessEventFd(int i)
{
	char buff[552];
	*logStream << "\t_clientSockets[" << i << "] had a revent, fd = " << _clientSockets[i].fd;
	*logStream << " | REVENTS == " << _clientSockets[i].revents << std::endl;
	if (_clientSockets[i].fd == 0)
		this->executeMachCmds(buff);
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
			*logStream << "\treceived mssg = " << buff;
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
	/*
	else
		servLogMssg("poll didn't found any events in given fd's");
	*/
}

// accepting new client, by verifying if the NickName isn't in use already
void	Server::acceptClient()
{
    socklen_t len = sizeof(_client);
    int	client_fd = 0;
    char buff[552];
    buff[551] = 0;
    std::string temp;
    client_fd = accept(_entrySocket, (struct sockaddr *)&_client, &len);
    if (client_fd >= 0)
    {

	/**************************************************************************/

	char buff[2];
	static int x = 47;
	x++;
	buff[0] = x;
	buff[1] = 0;


	std::string str = std::string("test user number #") + std::string(buff);

	/**************************************************************************/

		buff[recv(client_fd, (void*)buff, 551, 0)] = 0;
		*logStream << "(SERVER): new client try to join, the client message:" << std::endl << buff;
		Client &tmp = *(new Client(this, "test user", client_fd));
		this->addClient(tmp, client_fd);
		this->sendToClient(tmp, "Welcome to our first IRC server for 42.paris!");
		parseClientSent(buff, tmp);
    }
	/*
    else
    	serverLogMssg(" no new client tryed to join");
	*/
}

// adding new client to the existing one's
void	Server::addClient(Client const & user, int fd)
{
	struct pollfd *tmp = (struct pollfd*)malloc(sizeof(struct pollfd));
	struct pollfd &tmp2 = *tmp;
	tmp->fd = fd;
	tmp->events = POLLIN;
	tmp->revents = 0;
	_clientSockets.push_back(tmp2);
	_usersMap.insert(std::make_pair(fd, user));
	serverLogMssg("new client added");
}

// deleting client from the server
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