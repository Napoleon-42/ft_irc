#ifndef Oper_HPP
# define Oper_HPP

#include "../Command.hpp"


class Oper : public Command {
    protected :
        Client *_user;
    public :
        Oper();
        Oper(Client &user);

        std::string help_msg();
        void    execute(std::string line);
}

#endif