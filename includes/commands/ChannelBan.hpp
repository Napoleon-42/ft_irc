#ifndef ChannelBan_HPP
# define ChannelBan_HPP

#include "../Command.hpp"

class ChannelBan : public Command {
    protected :
        Client *_user;
    public :
        ChannelBan();
        ChannelBan(Client &user);

        std::string help_msg();
        void    execute(std::string line);
}

#endif