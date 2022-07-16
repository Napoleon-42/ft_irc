#include "commands/Kick.hpp"
#include "Server.hpp"

Kick::Kick() {
}

Kick::Kick(Server *serv) : Command(serv) {
}

std::string Kick::help_msg() const {
    return ("/kick username (allows you to kick this user from the channel)");
}

void Kick::execute(std::string line, Client &user) {
    Channel *chan = user.getChannel();
    if (!chan)
        return ;
    Client *tokick = chan->searchClient(line);
    if (!tokick) {
        _serv->sendToClient(user, "There is no user with this name on this channel.");
        return ;
    }
    chan->kickFromChannel(tokick);
}