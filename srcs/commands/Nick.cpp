/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:52 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/27 18:12:50 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Nick.hpp"
#include "Client.hpp"

Nick::Nick() {
}

Nick::Nick(Server *serv) : Command(serv) {

}

std::string Nick::help_msg() const {
    return ("/nick <nickname> (allows you to change your nickname in the server)");
}

void Nick::execute(std::string line, Client &user) {
    std::string channame = line;
    std::vector<std::string> params = ftirc_split(line, " ");
    if (params.size() == 0)
    {
        user.receive_reply(461, "NICK");
        return;
    }
    if (_serv->searchClient(line))
<<<<<<< HEAD
        user.receive_reply(433, user.getNname());
=======
    {
        user.receive_reply(433, line);
        return ;
    }
>>>>>>> f4338b58d8ef207a9cb458a18a5e888b4f606002
    if (!user.isPending())
    {
        for (Server::clientmap::const_iterator it = _serv->getClients().begin(); it != _serv->getClients().end(); ++it)
            _serv->sendToClient(it->second, ":" + user.getPrefix(), "NICK :" + line);
    }
//    else{
//        _serv->sendToClient(user, ":" + user.getPrefix(), "NICK :" + line);
//    }
    user.validateNick();
    user.changeName(line);
    serverLogMssg("Nick command executed");
}

const char * Nick::NameTakenException::what() const throw() {
    return "nickname in use";
}