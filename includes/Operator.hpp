#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include "ft_irc.hpp"
#include "Client.hpp"

class Operator : public Client
{
	public:

		Operator(): Client("non-spec")
		{
			this->commands.insert(std::make_pair("kban", new ChannelBan(this)));
		}
		
		Operator(Client user) : Client(_userName) {
			this->commands.insert(std::make_pair("kban", new ChannelBan(this)));
		}

		~Operator()
		{
		}
};

#endif