/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:47:37 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/12 19:14:21 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "commands/Nick.hpp"
#include "commands/Help.hpp"
#include "commands/Oper.hpp"
#include "commands/Join.hpp"
#include "commands/List.hpp"
#include "commands/Usercmd.hpp"
#include "commands/ChannelBan.hpp"

class Server
{
	public:
		typedef std::map<std::string, Command *>	commandmap;
		typedef std::map<std::string, Channel>		channelmap;
		typedef std::map<int, Client>				clientmap;
	
		int _entrySocket;
		std::vector<int> _clientSockets;
		clientmap			_usersMap;
		struct sockaddr_in _address;
		struct sockaddr_in _client;
		std::string		_passop;
		commandmap		_servercommands;
		commandmap		_opcommands;
		channelmap		_channels;

	public:

		Server()
		{
			//serverLogMssg("Server created");
			init_socket();
			_servercommands.insert(std::make_pair("NICK", new Nick(this)));
			_servercommands.insert(std::make_pair("OPER", new Oper(this)));
			_servercommands.insert(std::make_pair("HELP", new Help(this)));
			_servercommands.insert(std::make_pair("JOIN", new Join(this)));
			_servercommands.insert(std::make_pair("LIST", new List(this)));
			_servercommands.insert(std::make_pair("USER", new Usercmd(this)));
			/* TO DO
			INFO [<target>]
			INVITE <nickname> <channel>
			ISON <nicknames>
			PASS <password>
			PRIVMSG <msgtarget> :<message>
			QUIT [<message>]
			USER <username> <hostname> <servername> <realname>
			*/
    		_opcommands.insert(std::make_pair("BAN", new ChannelBan(this)));
			/* TO DO
			MUTE <nicknames ?>
			KICK <channel> <client> :[<message>] (does not ban just kick)
			KILL <client> <comment>
			DIE (command to shutdown server)
			*/
		}

		~Server()
		{
			//serverLogMssg("Server downed");
		}

		const commandmap &getServCommands() const {
			return (_servercommands);
		}

		const commandmap &getOpCommands() const {
			return (_opcommands);
		}

		const channelmap &getChannels() const {
			return(_channels);
		}

		const clientmap &getClients() const {
			return(_usersMap);
		}

		std::string		&serverhash(std::string &toHash) const {
			return (toHash);
		}

		bool			checkOpPass(std::string pass) const {
			if (serverhash(pass) == _passop)
				return (true);
			return (false);
		}

		channelmap::iterator addChannel(Channel &newchan) {
			return _channels.insert(std::make_pair(newchan.getName(), newchan)).first;

		}

		Channel *searchChannel(std::string channame) {
			channelmap::iterator it = _channels.find(channame);
			if (it == _channels.end())
				return (NULL);
			return (&(it->second));
		}
		
		void	init_socket()
		{
			_entrySocket = socket(AF_INET, SOCK_STREAM, 0);
			serverLogMssg("socket created");
			
			memset(&_address, 0, sizeof(_address));
			_address.sin_family = AF_INET;
			_address.sin_addr.s_addr = htonl(INADDR_ANY);
			_address.sin_port = htons(8080);

			if ((bind(_entrySocket, (struct sockaddr *)&_address, sizeof(_address))) != 0)
			{
				//serverLogMssg("socket bind failure");
				return ;
			}
			else
				//serverLogMssg("socket bind succeded");

			if ((listen(_entrySocket, 5)) != 0)
			{
				//serverLogMssg("listen() failure");
				return ;
			}
			else
				serverLogMssg("server is listening");
			return ;
		}


		void	acceptClients()
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
				//std::cin >> temp;
				//if (temp == "exit")
				//	_exit(0);
			}
		}

};



#endif