/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:54:18 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/15 16:00:21 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <string>

#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <poll.h>
#include <vector>
#include <map>

#define LOG_FILE "ft_irc.log"
#define FILE_OUTPUT 0
#define STD_OUTPUT 1

#define SERVER_NAME ":ft_irc.42.fr"

extern	std::ostream * logStream;

/*****************************************************************************/
/*									UTILS									 */
/*****************************************************************************/

void	serverLogMssg(std::string to_print);
void	clientLogMssg(std::string to_print);




#endif