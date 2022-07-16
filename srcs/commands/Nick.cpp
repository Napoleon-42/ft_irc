/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:52 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:53 by lnelson          ###   ########.fr       */
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
    user.changeName(line);
    serverLogMssg("Nick command executed");
}