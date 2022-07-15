/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:47 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:49 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Help.hpp"
#include "Server.hpp"

Help::Help() {
}

Help::Help(Server *serv) : Command(serv) {

}

std::string Help::help_msg() const {
    return ("/help command (show usage of command)");
}

void Help::execute(std::string line, Client &user) {
    Command *cmd = user.searchCommand(line);
    if (cmd)
        _serv->sendToClient(user, cmd->help_msg());
}