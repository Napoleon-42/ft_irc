#include "Client.hpp"

void        Client::addBasicCommands() {
    commands.insert(std::make_pair("nick", Nick(this)));
    commands.insert(std::make_pair("oper", Oper(this)));
    commands.insert(std::make_pair("help", Help(this)));
    /* TO DO
    INFO [<target>]
    INVITE <nickname> <channel>
    ISON <nicknames>
    JOIN <channels> [<keys>]
    LIST [<channels> [<server>]]
    PASS <password>
    PRIVMSG <msgtarget> :<message>
    QUIT [<message>]
    USER <username> <hostname> <servername> <realname>
    */
}

void        Client::addOpCommands() {
    this->commands.insert(std::make_pair("kban", ChannelBan(this)));
    /* TO DO
    KICK <channel> <client> :[<message>] (does not ban just kick)
    KILL <client> <comment>
    DIE (command to shutdown server)
    */
}

Client::Client(): _userName("non-spec")
{
    clientLogMssg(std::string("Client " + _userName + " created"));
    addBasicCommands();
}

Client::Client(Server *current, std::string uname, std::string hname, std::string sname) :
    _serv(current), _userName(uname), _hostname(hname), _servername(sname)
{
    clientLogMssg(std::string("Client " + _userName + " created"));
    addBasicCommands();

}

Client::~Client()
{
    clientLogMssg(" Client " + _userName + " destroyed");
    
}

void	    Client::becomeOperator() {
    this->addOpCommands();
}

void	    Client::execute(std::string &command, std::string &restline) {
    commands::iterator cit = commands.find(command);
    if (cit == commands.end())
        std::cout << "Command '" << command <<"' not found in available commands for the client : " << _userName << std::endl;
    cit->execute(restline);
}

Command&    Client::searchCommand(std::string cmd) {
    commands::iteraor cit = commands.find(command);
    if (cit == commands.end()) {
        std::cout << "Command '" << command <<"' not found in available commands for the client : " << _userName << std::endl;
        return (NULL);
    }
    return (*cit);
}

void        Client::becomeOperator() {
    this = new Operator(this);
}