

#include "Part.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Part::Part()
{
}

Part::Part(const Part&src)
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Part::~Part()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Part	&Part::operator=(Part const &rhs)
{
	return *this;
}

std::ostream &operator<<(std::ostream &o, Part const &instance)
{
	//o << instance.value;
	//return (o);
}
/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
