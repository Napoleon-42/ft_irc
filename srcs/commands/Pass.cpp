#include "commands/Pass.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Pass::Pass() {
}

Pass::Pass(Server *serv) : Command(serv) {

}

std::string Pass::help_msg() const {
    return ("/Pass <password> (check password to access server)");
}

void Pass::execute(std::string line, Client &user) {
    if (!_serv->checkServerPass(line))
    { // Connection refusée
        throw WrongPassException();
    }
    else
    { // Connection réussie
        clientLogMssg("Bon mdp rentré.");
        _serv->sendToClient(user, "Welcome !");
    }
}


const char * Pass::WrongPassException::what() const throw() {
    return "wrong_pass";
}