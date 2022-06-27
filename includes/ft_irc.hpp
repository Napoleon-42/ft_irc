/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:54:18 by lnelson           #+#    #+#             */
/*   Updated: 2022/06/27 14:16:46 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <string>

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

#define LOG_FILE "ft_irc.log"
#define FILE_OUTPUT 0
#define STD_OUTPUT 1

extern	std::ostream * logStream;

class Server
{
	private:
	
		int _socket;
		struct sockaddr_in _client;
		struct sockaddr_in _address;

	public:

		Server()
		{
			*logStream << "Server created" << std::endl;
			init_socket();
		}

		~Server()
		{
			*logStream << "Server downed" << std::endl;
		}

		void	init_socket()
		{
			_socket = socket(AF_INET, SOCK_STREAM, 0);
			*logStream << "Serv_mssg: socket created" << std::endl;
			
			memset(&_address, 0, sizeof(_address));
			_address.sin_family = AF_INET;
			_address.sin_addr.s_addr = htonl(INADDR_ANY);
			_address.sin_port = htons(8080);

			if ((bind(_socket, (struct sockaddr *)&_address, sizeof(_address))) != 0)
			{
				*logStream << "Serv_mssg: Bind failed!" << std::endl;
				*logStream << std::endl;
				return ;
			}
			else
				*logStream << "Serv_mssg: Bind succes!" << std::endl;

			if ((listen(_socket, 5)) != 0)
			{
				*logStream << "(SERVER): listen failed!" << std::endl;
				*logStream << std::endl;
			}
			else
				*logStream << "(SERVER): server is listening!" << std::endl;
		
			socklen_t len = sizeof(_client);
			int client_fd;
			while (1)
			{
				std::string temp;
				client_fd = accept(_socket, (struct sockaddr *)&_client, &len);
				if (client_fd >= 0)
				{
					*logStream << "(SERVER): new client accepted" << std::endl;
					send(client_fd, "Hello, world!", 13, 0);
				}
				
				std::cin >> temp;
				if (temp == "exit")
					_exit(0);
			}

			*logStream << std::endl;
			return ;
		}

};

class Client
{
	private:

		std::string _userName;

	public:

		Client()
		{
			*logStream << "Client created" << std::endl;
		}
		
		~Client()
		{
			*logStream << "Client destructed" << std::endl;
		}
};

#endif