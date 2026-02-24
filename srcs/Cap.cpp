#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Utils.hpp"

bool Command::cap(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string subcommand;

    if (args.size() < 2) 
        return true;

    subcommand = Utils::to_upper(args[1]);
    if (subcommand == "LS")
    {
        reply = ":" + server.get_name() + " CAP * LS :\r\n"; 
        client.send(reply);
        return true;
    }
    return true;
}