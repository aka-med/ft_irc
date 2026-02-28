#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::quit(Server &server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    std::string message = args.size() > 1 ? Utils::join(args, 1, " ") : "Client Quit";
    reply = Reply::format_command(client, "QUIT", "", message);

    client->send(reply);
    if (client->get_channels_count() > 0)
    {
        std::vector<std::string> channels = client->get_channels_list();
        for (size_t i = 0; i < channels.size(); i++)
        {
            Channel *channel = server.getChannel(channels[i]);
            if (channel)
                channel->broadcast(reply, client);
        }
    }
    return Client::REMOVE_CLIENT_SOCKET;
}