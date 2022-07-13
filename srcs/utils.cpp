/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 17:37:07 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 19:09:00 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void	serverLogMssg(std::string to_print)
{
	*logStream << "(SERVER): " << to_print << std::endl;
}

void	clientLogMssg(std::string to_print)
{
	*logStream << "(CLIENT): " << to_print << std::endl;
}

/*
void	inserNewClient(std::map<int, Client> usesMap, char buff[552])
{
	std::string uname;
	std::string hostName;
	std::string realName;

	
}
*/
