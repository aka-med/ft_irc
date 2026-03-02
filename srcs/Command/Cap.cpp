#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::cap(Server *server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string subcommand;

    if (args.size() < 2) 
        return Client::KEEP_CLIENT_SOCKET;

    subcommand = Utils::to_upper(args[1]);
    if (subcommand == "LS")
    {
        reply = ":" + server->get_name() + " CAP * LS :\r\n"; 
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    return Client::KEEP_CLIENT_SOCKET;
}