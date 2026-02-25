#include "../includes/Reply.hpp"
#include "../includes/Client.hpp"

std::string Reply::format_reply(const std::string &prefix, const std::string &numeric, const std::string &target, const std::string &parameters, const std::string &trailing)
{
    std::string reply = ":" + prefix + " " + numeric + " " + target;
    if (!parameters.empty())
        reply += " " + parameters;
    if (!trailing.empty())
        reply += " :" + trailing;
    reply += "\r\n";
    return reply;
}

std::string Reply::format_command(const Client &sender, const std::string &command, const std::string &params, const std::string &trailing)
{
    std::string msg = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + sender.get_ip();
    msg += " " + command;
    if (!params.empty())
        msg += " " + params;
    if (!trailing.empty())
        msg += " :" + trailing;
    msg += "\r\n";
    return msg;
}
