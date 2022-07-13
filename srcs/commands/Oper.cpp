/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:55 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:57 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Oper.hpp"
#include "Client.hpp"

Oper::Oper() {
}

Oper::Oper(Server *serv) : Command(serv) {

}

std::string Oper::help_msg() const {
    return ("/oper (allows you to become an operator in the server)");
}

void Oper::execute(std::string line, Client &user) {
    (void)line; //need to check if he used operator password
    user.becomeOperator();
}