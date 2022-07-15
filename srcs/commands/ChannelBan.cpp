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
    return ("/kban username (allows you to ban this user from the channel)");
}

void ChannelBan::execute(std::string line, Client &user) {
    Channel *chan = user.getChannel();
    if (!chan)
        return ;
    Client *toban = chan->searchClient(line);
    if (!toban) {
        clientLogMssg("There is no user with this name on this channel.");
        return ;
    }
    chan->addToBanList(toban);
    chan->kickFromChannel(toban);
}