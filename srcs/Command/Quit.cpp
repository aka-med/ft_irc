#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::quit(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client.get_nick().empty() ? "*" : client.get_nick();
    if (!client.is_authenticated())
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOTREGISTERED, target, "", "You have not registered");
        client.send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    std::string message = args.size() > 1 ? Utils::join(args, 1, " ") : "Client Quit";
    reply = Reply::format_command(client, "QUIT", "", message);

    client.send(reply);
    if (client.get_channels_count() > 0)
    {
        const std::vector<std::string> &channels = client.get_channels_list();
        for (size_t i = 0; i < client.get_channels_count(); ++i)
        {
            Channel *channel = server.getChannel(channels[i]);
            if (channel)
            {
                channel->broadcast(reply, client);
                channel->remove_user(client);
                client.remove_channel_from_client(channels[i]);
                if (channel->is_operator(client))
                    channel->remove_operator(client);
            }
        }
    }
    return Client::REMOVE_CLIENT_SOCKET;
}