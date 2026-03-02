#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::privmsg(Server *server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if (args.size() < 3)
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NEEDMOREPARAMS, target, "PRIVMSG", "Not enough parameters");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    std::string recipient = args[1];
    std::string message = Utils::join(args, 2, " ");
    if (!recipient.empty() && recipient[0] == '#')
    {
        if (!server->channelExists(recipient))
        {
            reply = Reply::format_reply(server->get_name(), Reply::ERR_NOSUCHCHANNEL, target, recipient, "No such channel");
            client->add_to_queue(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        Channel *channel = server->getChannel(recipient);
        if (!channel->is_user_in_channel(client))
        {
            reply = Reply::format_reply(server->get_name(), Reply::ERR_CANNOTSENDTOCHAN, target, recipient, "Cannot send to channel");
            client->add_to_queue(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        channel->broadcast(Reply::format_command(client, "PRIVMSG", recipient, message), client);

        std::string botReply = server->getBot().processChannelMessage(client->get_nick(), recipient, message);
        if (!botReply.empty())
        {
            Client botClient;
            botClient.set_server(server);
            botClient.set_nick(server->getBot().getNick());
            botClient.set_fd(-1);
            botClient.set_full_connected(true);
            botClient.set_authenticated(true);
            channel->broadcast(botReply, &botClient);
        }
    }
    else
    {
        Client *recipient_client = server->getClientByNick(recipient);
        if (!recipient_client)
        {
            reply = Reply::format_reply(server->get_name(), Reply::ERR_NOSUCHNICK, target, recipient, "No such nick");
            client->add_to_queue(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }

        reply = Reply::format_command(client, "PRIVMSG", recipient, message);
        recipient_client->add_to_queue(reply);
    }
    return Client::KEEP_CLIENT_SOCKET;
}