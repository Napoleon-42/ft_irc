/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:06:57 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:07:32 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	init_socket();
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
    _entrySocket = socket(AF_INET, SOCK_STREAM, 0);
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

void	Server::acceptClients()
{
    socklen_t len = sizeof(_client);
    int	client_fd = 0;
    char buff[552];

    buff[551] = 0;
    while (1)
    {
        std::string temp;
        client_fd = accept(_entrySocket, (struct sockaddr *)&_client, &len);
        if (client_fd >= 0)
        {
            serverLogMssg(" new client accepted");
            _clientSockets.push_back(client_fd);
            _usersMap.insert(std::make_pair(client_fd, Client(this, "test client")));
            send(client_fd, "Hello world lnelson \r\n", 22, 0);
        }
        else
        {	
            serverLogMssg(" failure to accept new client");
            *logStream << client_fd << std::endl;
        }

        if (client_fd != 0)
        {
            buff[recv(client_fd, (void *)buff, 551, 0)] = 0;

            *logStream << "(SERVER) received mssg : "<< std::endl << buff << std::endl;
        }
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