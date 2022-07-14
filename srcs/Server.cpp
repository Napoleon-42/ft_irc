/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:06:57 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/14 13:46:03 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	init_socket();
	addClient(Client(this, "Server Machine Admin"), 0);
    _servercommands.insert(std::make_pair("nick", new Nick(this)));
	_servercommands.insert(std::make_pair("oper", new Oper(this)));
	_servercommands.insert(std::make_pair("help", new Help(this)));
    /* TO DO
    INFO [<target>]
    INVITE <nickname> <channel>
    ISON <nicknames>
    JOIN <channels> [<keys>]
    LIST [<channels> [<server>]]
    PASS <password>
    PRIVMSG <msgtarget> :<message>
    QUIT [<message>]
    USER <username> <hostname> <servername> <realname>
    */
    _opcommands.insert(std::make_pair("kban", new ChannelBan(this)));
    /* TO DO
    KICK <channel> <client> :[<message>] (does not ban just kick)
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
** --------------------------------- METHODS ----------------------------------
*/

void Server::init_socket()
{
    _entrySocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    serverLogMssg("socket created");
    
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    _address.sin_port = htons(8080);

    if ((bind(_entrySocket, (struct sockaddr *)&_address, sizeof(_address))) != 0)
    {
        serverLogMssg("socket bind failure");
        return ;
    }
    else
        serverLogMssg("socket bind succeded");

    if ((listen(_entrySocket, 5)) != 0)
    {
        serverLogMssg("listen() failure");
        return ;
    }
    else
        serverLogMssg("server is listening");
    return ;
}

void	Server::routine()
{
	char buff[552];
	while (1)
	{
		this->acceptClient();
		if (poll(&(*(_clientSockets.begin())), _clientSockets.size(), 500) > 0)
		{
			serverLogMssg(" pool detected something:");
			for (unsigned int i = 0; i < _clientSockets.size(); i++)
			{
				if (_clientSockets[i].revents != 0)
				{
					*logStream << "\t_clientSockets[" << i << "] had a revent, fd = " << _clientSockets[i].fd << std::endl;
					if (_clientSockets[i].fd == 0)
						buff[read(0, buff, 552)] = 0;
					else
						buff[recv(_clientSockets[i].fd, (void*)buff, 551,0)] = 0;
					*logStream << "\treceived mssg = " << buff;
					_clientSockets[i].revents = 0;
				}
			}
		}
		/*
		else
			serverLogMssg(" poll didn't detect new entry's");
		*/
	}
}


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
		buff[recv(client_fd, (void*)buff, 551, 0)] = 0;
		*logStream << "(SERVER): new client try to join, the client message:" << std::endl << buff;
		Client 
		this->addClient(Client(this, "test user"), client_fd);
    	send(client_fd, ":ft_irc.42.fr 001 lnelson :Welcome to our ft_irc server\r\n", 57, 0);
    }
	/*
    else
    	serverLogMssg(" no new client tryed to join");
	*/
}

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

Server::channelmap::iterator Server::addChannel(Channel &newchan)
{
	return _channels.insert(std::make_pair(newchan.getName(), newchan)).first;
}

Channel *Server::searchChannel(std::string channame)
 {
    channelmap::iterator it = _channels.find(channame);
    if (it == _channels.end())
        return (NULL);
    return (&(it->second));
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



/* ************************************************************************** */