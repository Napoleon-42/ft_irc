#include "commands/Oper.hpp"

Oper::Oper() {
}

Oper::Oper(Client &user) : Command(user) {

}

std::string Oper::help_msg() {
    return ("/oper (allows you to become an operator in the server)");
}

void Oper::execute(std::string line) {
    _user->becomeOperator();
}