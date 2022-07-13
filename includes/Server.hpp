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

		Server();
		~Server();
		const commandmap &getServCommands() const;
		const commandmap &getOpCommands() const;
		const channelmap &getChannels() const;
		channelmap::iterator addChannel(Channel &newchan);
		Channel *searchChannel(std::string channame);
		void	init_socket();
		void	acceptClients();
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
