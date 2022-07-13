/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:48:04 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/12 19:11:55 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <map>
#include <string>
/*
** Pre-declared class needed to initialize Clients
*/
class Server;
class Command;
class Channel;

class Client
{
	private:
		typedef std::map<std::string, Command *> commandmap;

		std::string		_userName;
		std::string		_hostname;
		std::string		_servername;
		Server*			_serv;
		Channel*		_currentChannel;
		commandmap		commands;

		void addBasicCommands();
		void addOpCommands();
	public:

		Client();
		Client(Server *current, std::string uname);
		Client(Server *current, std::string uname, std::string hname, std::string sname);
		~Client();

		void	execute(std::string &command, std::string &restline);

		Command *searchCommand(std::string cmd);

		void	becomeOperator();
		void	changeName(std::string &newname) {
			_userName = newname;
		}
		void	changeChannel(Channel *chan) {
			_currentChannel = chan;
		}
		/*
		** getters
		*/

		std::string getUname() {
			return (_userName);
		}
		Channel *getChannel() {
			return (_currentChannel);
		}
		Server *getServer() {
			return (_serv);
		}
};

#include "Server.hpp"
#include "Command.hpp"
#include "Channel.hpp"

#endif