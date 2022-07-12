/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:47:37 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/12 17:28:45 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "ft_irc.hpp"

class Server
{
	private:
	
		int _socket;
		struct sockaddr_in _client;
		struct sockaddr_in _address;

	public:

		Server()
		{
			serverLogMssg("Server created");
			init_socket();
		}

		~Server()
		{
			serverLogMssg("Server downed");
		}

		void	init_socket()
		{
			_socket = socket(AF_INET, SOCK_STREAM, 0);
			serverLogMssg("socket created");
			
			memset(&_address, 0, sizeof(_address));
			_address.sin_family = AF_INET;
			_address.sin_addr.s_addr = htonl(INADDR_ANY);
			_address.sin_port = htons(8080);

			if ((bind(_socket, (struct sockaddr *)&_address, sizeof(_address))) != 0)
			{
				serverLogMssg("socket bind failure");
				return ;
			}
			else
				serverLogMssg("socket bind succeded");

			if ((listen(_socket, 5)) != 0)
			{
				serverLogMssg("listen() failure");
				return ;
			}
			else
				serverLogMssg("server is listening");
			
			(void)_client;
			/*
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
			*/
			
			return ;
		}

};



#endif