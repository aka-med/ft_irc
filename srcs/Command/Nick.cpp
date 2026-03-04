#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::nick(Server *server, Client *client, const std::string &nickname)
{
    std::string reply;
    std::string target = client->get_nick().empty() ? "*" : client->get_nick();
    std::string old_nick = client->get_nick();
    if(!client->is_authenticated())
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NOTREGISTERED, target, "", "You have not registered");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (nickname.empty())
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NONICKNAMEGIVEN, target, "", "No nickname given");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (!Utils::validate_nick(nickname))
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_ERRONEUSNICKNAME, target, nickname, "Erroneous nickname");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (server->isNickTaken(nickname, client->get_fd()))
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NICKNAMEINUSE, target, nickname, "Nickname is already in use");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }

    if (!old_nick.empty())
    {
        reply = Reply::format_command(client, "NICK", nickname, "");
        client->add_to_queue(reply);
        const std::vector<std::string> &channels = client->get_channels_list();
        for (size_t i = 0; i < channels.size(); i++)
        {
            Channel *chan = server->getChannel(channels[i]);
            if (chan)
                chan->broadcast(reply, client);
        }
        client->set_nick(nickname);
        return Client::KEEP_CLIENT_SOCKET;
    }
    
    client->set_nick(nickname);
    target = nickname;
    if (!client->get_user().empty() && !client->full_connected())
    {
        client->set_full_connected(true);
        reply = Reply::format_reply(server->get_name(), Reply::RPL_WELCOME, target, "", 
            "Welcome to the Internet Relay Network " + target + "!" + client->get_user() + "@" + client->get_ip());
        client->add_to_queue(reply);

        reply = Reply::format_reply(server->get_name(), Reply::RPL_YOURHOST, target, "", 
            "Your host is " + server->get_name() + ", running version 1.0");
        client->add_to_queue(reply);
        
        reply = Reply::format_reply(server->get_name(), Reply::RPL_CREATED, target, "", 
            "This server was created today");
        client->add_to_queue(reply);

        reply = Reply::format_reply(server->get_name(), Reply::RPL_MYINFO, target, 
                    server->get_name() + " 1.0 " + "" + " itkol", "is the info for this server");
        client->add_to_queue(reply);

        reply = Reply::format_reply(server->get_name(), Reply::ERR_NOMOTD, target, "", "MOTD File is missing");
        client->add_to_queue(reply);
    }
    return Client::KEEP_CLIENT_SOCKET;
}