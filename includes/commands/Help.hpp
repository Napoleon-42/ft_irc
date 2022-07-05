#ifndef Help_HPP
# define Help_HPP

#include "../Command.hpp"

class Help : public Command {
    protected :
        Client *_user;
    public :
        Help();
        Help(Client &user);

        std::string help_msg() const;
        void    execute(std::string line);
};

#endif