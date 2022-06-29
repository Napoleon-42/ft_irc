#ifndef Nick_HPP
# define Nick_HPP

#include "../Command.hpp"


class Nick : public Command {
    protected :
        Client *_user;
    public :
        Nick();
        Nick(Client &user);

        std::string help_msg();
        void    execute();
}

#endif