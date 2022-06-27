/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 21:28:48 by lnelson           #+#    #+#             */
/*   Updated: 2022/06/27 15:41:26 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include <fstream>

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

int main(int ac, char **av)
{
	/**************************************************************************/
	
	logStream = set_log_file(ac == 2 ? FILE_OUTPUT : STD_OUTPUT);
	(void)ac;
	(void)av;

	/***************************************************************************/
	
	Server irc;
	Client client;

	std::string temp;
	while(1)
	{	
		std::cin >> temp;
		if (temp == "exit")
			_exit(0);
	}
}