#include "commands/Oper.hpp"

Oper::Oper() {
}

Oper::Oper(Server &serv) : Command(serv) {

}

std::string Oper::help_msg() const {
    return ("/oper (allows you to become an operator in the server)");
}

void Oper::execute(std::string line, Client &user) {
    user->becomeOperator();
}