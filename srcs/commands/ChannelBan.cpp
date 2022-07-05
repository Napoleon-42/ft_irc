#include "commands/ChannelBan.hpp"

ChannelBan::ChannelBan() {
}

ChannelBan::ChannelBan(Client &user) : Command(user) {
}

std::string ChannelBan::help_msg() const {
    return ("/kban username (allows you to ban this user from the channel)");
}

void ChannelBan::execute(std::string line) {
    //add to ban list so he can't rejoin channel
    //then kick him out of channel
}