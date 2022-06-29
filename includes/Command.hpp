#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Client.hpp"

class Command {
    protected :
        Client *_user;
    public :
        Command() {}
        Command(Client &user) : _user(&user) {}

        bool hasUser() {
            if (_user)
                return (true);
            return (false);
        }
        std::string help_msg() = 0;
        void    execute() = 0;
}

#endif