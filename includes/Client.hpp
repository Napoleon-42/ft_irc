/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:48:04 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/31 13:07:48 by lnelson          ###   ########.fr       */
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
	public:
		typedef std::map<std::string, Command *> commandmap;
	
	private:
		Server*			_serv;
		std::string		_username;
		std::string		_nickname;
		std::string		_hostname;
		std::string		_servername;
		std::string		_realname;
		int				_fd;
		Channel*		_currentChannel;
		commandmap		_commands;
		bool			_pwdPass;
		bool			_nick;
		bool			_user;

		void addLoginCommands();
		void addOpCommands();

	public:

		Client();
		Client(Server *current, std::string uname);
		Client(Server *current, std::string uname, int fd);
		Client(Server *current, std::string uname, std::string hname, std::string sname);
		Client(Client const & var);
		~Client();

		bool	execute(std::string const &command, std::string const &restline);

		Command *searchCommand(std::string cmd);

		void addBasicCommands();
		void	becomeOperator();
		void	changeName(std::string newname) {
			_nickname = newname;
		}
		void	update_all_name(std::string uname, std::string hname, std::string sname, std::string rname) {
			_username = uname;
			_hostname = hname;
			_servername = sname;
			_realname = rname;
		}
		void	changeChannel(Channel *chan) {
			_currentChannel = chan;
		}
		/*
		** getters
		*/

		void	validatePass()
		{
			_pwdPass = true;
		}

		void	validateNick()
		{
			_nick = true;
		}

		void	validateUser()
		{
			_user = true;
		}


		int		getFd() const {
			return (_fd);
		}

		bool	getPassStatus() const{
			return (_pwdPass);
		}
		bool	getUserStatus() const{
			return (_user);
		}
		bool	getNickstatus() const{
			return (_nick);
		}

		const std::string &getUname() const {
			return (_username);
		}
		const std::string &getHname() const {
			return (_hostname);
		}
		const std::string &getSname() const {
			return (_servername);
		}
		const std::string &getRname() const {
			return (_realname);
		}
		const std::string &getNname() const {
			return (_nickname);
		}
		std::string getPrefix() const;
		const commandmap &getCommands() const {
			return (_commands);
		}
		Channel *getChannel() const {
			return (_currentChannel);
		}
		Server *getServer() const {
			return (_serv);
		}
};

#include "Server.hpp"
#include "Command.hpp"
#include "Channel.hpp"

#endif