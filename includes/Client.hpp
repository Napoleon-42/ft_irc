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

class Client
{
	private:

		std::string _userName;

	public:

		Client(): _userName("non-spec")
		{
			clientLogMssg(std::string("Client " + _userName + " created"));
		}
		
		~Client()
		{
			clientLogMssg(" Client " + _userName + " destroyed");
		}
};

#endif