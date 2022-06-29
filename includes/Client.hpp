/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:48:04 by lnelson           #+#    #+#             */
/*   Updated: 2022/06/27 15:48:40 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "ft_irc.hpp"

class Command;
class Nick;

class Client
{
	private:

		std::string _userName;
		std::map<std::string, Command *> commands;

	public:

		Client(): _userName("non-spec")
		{
			clientLogMssg(std::string("Client " + _userName + " created"));
			commands.insert(std::make_pair("nick", new Nick(this)));
			commands.insert(std::make_pair("oper", new Oper(this)));
		}
		
		~Client()
		{
			clientLogMssg(" Client " + _userName + " destroyed");
		}

		void	execute(std::string &command, std::string &restline) {
			commands::iteraor cit = commands.find(command);
			if (cit == commands.end())
				std::cout << "Command '" << command <<"' not found in available commands for the client : " << _userName << std::endl
			cit->execute(restline);
		}

		void	becomeOperator() {
			this = new Operator(this);
		}

		void	changeName(std::string &newname) {
			_userName = newname;
		}
};

#endif