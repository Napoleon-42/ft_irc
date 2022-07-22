/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:47:37 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/23 01:05:21 by lnelson          ###   ########.fr       */
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
#include "commands/Ping.hpp"
#include "commands/Kick.hpp"
#include "commands/Quit.hpp"
#include "commands/Usercmd.hpp"
#include "commands/PrivMsg.hpp"
#include "commands/ChannelBan.hpp"

class Server
{
	public:
		typedef std::map<std::string, Command *>	commandmap;
		typedef std::map<std::string, Channel>		channelmap;
		typedef std::map<int, Client>				clientmap;
	
		int _entrySocket;
		std::vector<struct pollfd> _clientSockets;
		struct sockaddr_in _client;

		std::map<int, Client> _usersMap;
		
		struct sockaddr_in _address;

		std::string		_server_pwd;
		std::string		_passop;
		commandmap		_servercommands;
		commandmap		_opcommands;
		channelmap		_channels;

		void	proccessEventFd(int i);
		void	pollRoutine();
		void	acceptClient();
		void	executeMachCmds(char * buff);
		void	parseClientSent(char * buff, Client &user);
		void	init_socket(int port);
		std::string		&serverhash(std::string &toHash) const;
		bool			checkOpPass(std::string pass) const;

	public:

		Server();
		Server(int port, std::string pwd);
		~Server();
		const commandmap &getServCommands() const;
		const commandmap &getOpCommands() const;
		const channelmap &getChannels() const;
		const clientmap &getClients() const;
		channelmap::iterator addChannel(Channel &newchan);
		Channel *searchChannel(std::string channame);
		void	routine();
		void	sendToClient(Client const &sendTo, std::string mssg);
		void	addClient(Client const & user, int fd);
		void	deleteClient(std::string uname);
};

#endif
