#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include "ft_irc.hpp"
#include "Client.hpp"

class Operator : public Client
{
	public:

		Operator(): _userName("non-spec")
		{
			OperatorLogMssg(std::string("Operator " + _userName + " created"));
		}
		
		~Operator()
		{
			OperatorLogMssg(" Operator " + _userName + " destroyed");
		}
};

#endif