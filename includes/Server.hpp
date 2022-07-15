/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:47:37 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/15 16:01:47 by lnelson          ###   ########.fr       */
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
		std::vector<struct pollfd> _clientSockets;
		struct sockaddr_in _client;

		std::map<int, Client> _usersMap;
		
		struct sockaddr_in _address;
		std::string		_passop;
		commandmap		_servercommands;
		commandmap		_opcommands;
		channelmap		_channels;

		void	proccessEventFd(int i);
		void	pollRoutine();
		void	acceptClient();
		void	executeMachCmds(char * buff);
		void	init_socket();

	public:

		Server();
		~Server();
		const commandmap &getServCommands() const;
		const commandmap &getOpCommands() const;
		const channelmap &getChannels() const;
		channelmap::iterator addChannel(Channel &newchan);
		Channel *searchChannel(std::string channame);
		void	routine();
		void	sendToClient(Client sendTo, std::string mssg);
		void	addClient(Client const & user, int fd);
		void	deleteClient(std::string uname);

		/*
			_servercommands.insert(std::make_pair("LIST", new List(this)));
			_servercommands.insert(std::make_pair("USER", new Usercmd(this)));
    		_opcommands.insert(std::make_pair("BAN", new ChannelBan(this)));

		*/

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
};

#endif
