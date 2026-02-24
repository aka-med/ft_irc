#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Utils.hpp"

bool Command::user(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client.get_nick().empty() ? "*" : client.get_nick();
    if (!client.is_authenticated())
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOTREGISTERED, target, "", "You have not registered");
        client.send(reply);
        return false;
    }
    if (client.full_conected())
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_ALREADYREGISTRED, target, "", "You are already registered");
        client.send(reply);
        return false;
    }
    if (args.size() < 5 || args[1].empty() || args[2].empty() || args[3].empty() || args[4].empty())
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "USER", "Not enough parameters");
        client.send(reply);
        return false;
    }
    client.set_realname(args[4]);
    client.set_user(args[1]);
    if (!client.get_nick().empty() && !client.full_conected())
    {
        client.set_full_connected(true);
        reply = Reply::format_reply(server.get_name(), Reply::RPL_WELCOME, target, "", 
            "Welcome to the Internet Relay Network " + target + "!" + client.get_user() + "@" + client.get_ip());
        client.send(reply);

        reply = Reply::format_reply(server.get_name(), Reply::RPL_YOURHOST, target, "", 
            "Your host is " + server.get_name() + ", running version 1.0");
        client.send(reply);
        
        reply = Reply::format_reply(server.get_name(), Reply::RPL_CREATED, target, "", 
            "This server was created today");
        client.send(reply);

        reply = Reply::format_reply(server.get_name(), Reply::RPL_MYINFO, target, 
                    server.get_name() + " 1.0 " + "" + " itkol", "is the info for this server");
        client.send(reply);

        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOMOTD, target, "", "MOTD File is missing");
        client.send(reply);
    }
    return true;
}