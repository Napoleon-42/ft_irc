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
    if (_serv->searchClient(line))
        throw NameTakenException();
    if (user.isPending()){
        _serv->sendToClient(user, ":" + user.getPrefix(), "NICK :" + line);
    }
    else {
        for (Server::clientmap::const_iterator it = _serv->getClients().begin(); it != _serv->getClients().end(); ++it)
            _serv->sendToClient(it->second, ":" + user.getPrefix(), "NICK :" + line);
    }
    user.changeName(line);
    serverLogMssg("Nick command executed");
}

const char * Nick::NameTakenException::what() const throw() {
    return "nickname in use";
}