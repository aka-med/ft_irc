#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Utils.hpp"

bool Command::mode(Server &server, Client &client, const std::vector<std::string> &args)
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
    if (args.size() < 2)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "MODE", "Not enough parameters");
        client.send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (args[1][0] == '#')
    {
        // Mohammed Akarkaou :
        // here you should handel the MODE for channels 
        return Client::KEEP_CLIENT_SOCKET;
    }
    else
    {
        // just for skip the fist check from irssi
        reply = ":" + client.get_nick() + " MODE " + client.get_nick() + " :" + args[2] + "\r\n";
        client.send(reply);
    }
    return Client::KEEP_CLIENT_SOCKET;
}
