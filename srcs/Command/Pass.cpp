#include "../../includes/Commands.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Reply.hpp"


bool Command::pass(Server *server, Client *client, const std::string &password)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if (client->is_authenticated())
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_ALREADYREGISTRED, target, "", "You may not reregister");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (password == server->get_password())
    {
        client->set_authenticated(true);
        return Client::KEEP_CLIENT_SOCKET;
    }
    reply = Reply::format_reply(server->get_name(), Reply::ERR_PASSWDMISMATCH, target, "", "Password incorrect");
    client->add_to_queue(reply);
    return Client::KEEP_CLIENT_SOCKET;
}