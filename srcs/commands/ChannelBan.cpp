/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelBan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:45 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:46 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/ChannelBan.hpp"
#include "Server.hpp"

ChannelBan::ChannelBan() {
}

ChannelBan::ChannelBan(Server *serv) : Command(serv) {
}

std::string ChannelBan::help_msg() const {
    return ("/ban username (allows you to ban this user from the channel)");
}

void ChannelBan::execute(std::string line, Client &user) {
    std::string channame = line;
    std::vector<std::string> params = ftirc_split(line, " ");
    if (params.size() == 0)
    {
        user.receive_reply(461, "BAN");
        return;
    }
    Channel *chan = user.getChannel();
    if (!chan)
        return ;
    Client *toban = chan->searchClient(line);
    if (!toban) {
        _serv->sendToClient(user, "There is no user with this name on this channel.");
        return ;
    }
    chan->addToBanList(toban);
    chan->kickFromChannel(toban);
}