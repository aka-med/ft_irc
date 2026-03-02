#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"
bool Command::ping(Server *server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if (args.size() < 2)
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NEEDMOREPARAMS, target, "PING", "Not enough parameters");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    reply = Reply::format_reply(server->get_name(), "PONG", target, "", args[1]);
    client->add_to_queue(reply);
    std::cout << "Server sent PONG to " << target << std::endl;
    return Client::KEEP_CLIENT_SOCKET;
}