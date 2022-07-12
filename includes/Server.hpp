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

class Server
{
	private:
	
		int _entrySocket;
		std::vector<int> _clientSockets;
		struct sockaddr_in _client;

		std::map<int, Client> _usersMap;
		
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
			_entrySocket = socket(AF_INET, SOCK_STREAM, 0);
			serverLogMssg("socket created");
			
			memset(&_address, 0, sizeof(_address));
			_address.sin_family = AF_INET;
			_address.sin_addr.s_addr = htonl(INADDR_ANY);
			_address.sin_port = htons(8080);

			if ((bind(_entrySocket, (struct sockaddr *)&_address, sizeof(_address))) != 0)
			{
				serverLogMssg("socket bind failure");
				return ;
			}
			else
				serverLogMssg("socket bind succeded");

			if ((listen(_entrySocket, 5)) != 0)
			{
				serverLogMssg("listen() failure");
				return ;
			}
			else
				serverLogMssg("server is listening");
			return ;
		}


		void	acceptClients()
		{
			socklen_t len = sizeof(_client);
			int	client_fd = 0;
			char buff[552];
			buff[551] = 0;
			while (1)
			{
				std::string temp;
				client_fd = accept(_entrySocket, (struct sockaddr *)&_client, &len);
				if (client_fd >= 0)
				{
					serverLogMssg(" new client accepted");
					_clientSockets.push_back(client_fd);
					_usersMap.insert(std::make_pair(client_fd, Client("test client")));
					send(client_fd, "Hello world lnelson \r\n", 22, 0);
				}
				else
				{	
					serverLogMssg(" failure to accept new client");
					*logStream << client_fd << std::endl;
				}

				if (client_fd != 0)
				{
					buff[recv(client_fd, (void *)buff, 551, 0)] = 0;

					*logStream << "(SERVER) received mssg : "<< std::endl << buff << std::endl;
				}
				//std::cin >> temp;
				//if (temp == "exit")
				//	_exit(0);
			}
		}

};



#endif