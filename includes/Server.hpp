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
#include "commands/ChannelBan.hpp"

class Server
{
	public:
		typedef std::map<std::string, Command *> commandmap;
		typedef std::map<std::string, Channel> channelmap;
	
		int _entrySocket;
		std::vector<int> _clientSockets;
		struct sockaddr_in _client;

		std::map<int, Client> _usersMap;
		
		struct sockaddr_in _address;
		commandmap		_servercommands;
		commandmap		_opcommands;
		channelmap		_channels;

	public:

		Server();
		~Server();

		const commandmap &getServCommands() const {
			return (_servercommands);
		}

		const commandmap &getOpCommands() const {
			return (_opcommands);
		}

		const channelmap &getChannels() const {
			return(_channels);
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
		void	init_socket();
		void	acceptClients();
};

#endif
