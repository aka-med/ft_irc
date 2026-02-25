#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Commands.hpp"

bool Command::unknown(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string reply = Reply::format_reply(server.get_name(), Reply::ERR_UNKNOWNCOMMAND, client.get_nick().empty() ? "*" : client.get_nick(), args[0], "Unknown command");
    client.send(reply);
    return Client::KEEP_CLIENT_SOCKET;
}