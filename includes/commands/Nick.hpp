#ifndef Nick_HPP
# define Nick_HPP

#include "../Command.hpp"

class Nick : public Command {
    public :
        Nick();
        Nick(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif
