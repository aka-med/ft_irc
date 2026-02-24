#include "../includes/Reply.hpp"

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