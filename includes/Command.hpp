#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Client.hpp"

class Command {
    protected :
        Client *_user;
    public :
        Command() {}
        Command(Client &user) : _user(&user) {}
        virtual ~Command() {}

        bool hasUser() {
            if (_user)
                return (true);
            return (false);
        }

        virtual std::string help_msg() const = 0;
        virtual void    execute(std::string line) = 0;
};

#endif