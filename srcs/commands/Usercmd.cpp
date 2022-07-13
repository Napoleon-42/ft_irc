#include "commands/Usercmd.hpp"
#include "Client.hpp"

Usercmd::Usercmd() {
}

Usercmd::Usercmd(Server *serv) : Command(serv) {

}

std::string Usercmd::help_msg() const {
    return ("/Usercmd newname (allows you to change your username in the server)");
}

std::vector<std::string> ftirc_split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

void Usercmd::execute(std::string line, Client &user) {
    std::vector<std::string> splits = ftirc_split(line, " \t");
    if (splits.size() == 4)
    {
        user.update_all_name(splits[0], splits[1], splits[3], splits[4]);
        serverLogMssg("Right number of arguments. USER executed");
    }
    else {
        serverLogMssg("Wrong number of arguments for USER");
    }
}