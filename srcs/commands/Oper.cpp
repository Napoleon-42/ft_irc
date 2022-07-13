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