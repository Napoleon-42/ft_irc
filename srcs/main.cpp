/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 21:28:48 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:32:12 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include <fstream>
#include "Server.hpp"
#include "Client.hpp"

std::ostream * logStream;

std::ostream *	set_log_file(short key)
{
	if (key == STD_OUTPUT) 
	{
		std::cout << "#####		LOGS OUTPUTED IN STANDART OUTPUT		#####" << std::endl << std::endl;
		return (&(std::cout));
	}
	
	std::ostream *outFile = new std::ofstream();
	((std::ofstream*)(outFile))->open(LOG_FILE, std::ofstream::out);
	*outFile << "#####		LOGS OUTPUTED IN A LOG FILE				#####" << std::endl << std::endl;
	
	return (outFile);
}

/*
void	serverLogMssg(std::string to_print)
{
	*logStream << "(SERVER): " << to_print << std::endl;
}

void	clientLogMssg(std::string to_print)
{
	*logStream << "(CLIENT): " << to_print << std::endl;
}
*/

int main(int ac, char **av)
{
	/**************************************************************************/
	
	logStream = set_log_file(ac == 2 ? FILE_OUTPUT : STD_OUTPUT);
	(void)ac;
	(void)av;

	/***************************************************************************/
	
	Server irc;
	Channel chan(&irc, "chan1");
	Channel *ret;
	Client client(&irc, "Client1");

	irc.addChannel(chan);
	ret = irc.searchChannel("chan1");
	chan.addClient(&client);
	std::cout << chan.searchClient("Client1") << std::endl;
	std::cout << chan.addToBanList(&client) << std::endl;
	std::cout << chan.searchBanned("Client1") << std::endl;
	chan.kickFromChannel(&client);
	std::cout << chan.searchClient("Client1") << std::endl;
	*logStream << ret << std::endl;
	chan.addClient(&client);

	irc.routine();


	/***************************************************************************/
	/***************************************************************************/
}