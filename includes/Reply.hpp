#ifndef REPLY_HPP
#define REPLY_HPP

# include <string>

namespace Reply
{
    // 001 - 099 : Client Connection and Registration
    const std::string RPL_WELCOME = "001";
    const std::string RPL_YOURHOST = "002";
    const std::string RPL_CREATED = "003";
    const std::string RPL_MYINFO = "004";

    // 100 - 199 : (Unused in standard IRC)

    // 200 - 299 : Server Information, Stats, and Trace Data

    // 300 - 399 : Data Lists, Queries, and MOTD (NOT Errors!)
    
    // 400 - 499 : Standard Client and Command Errors
    const std::string ERR_NOMOTD = "422"; // For missing MOTD file
    const std::string ERR_NONICKNAMEGIVEN = "431"; // For missing nickname
    const std::string ERR_ERRONEUSNICKNAME = "432"; // For erroneous nickname
    const std::string ERR_NICKNAMEINUSE = "433"; // For nickname already in use
    const std::string ERR_NOTREGISTERED = "451"; // For commands that require registration
    const std::string ERR_NEEDMOREPARAMS = "461"; // For missing parameters in commands
    const std::string ERR_ALREADYREGISTRED = "462"; // For already registered clients
    const std::string ERR_PASSWDMISMATCH = "464"; // For server connection
    const std::string ERR_BADCHANNELKEY = "475";  // For channel joining
    
    // 500 - 599 : Server-Side Errors and Mode Errors
    const std::string ERR_USERSDONTMATCH = "502"; // For mode changes on other users


    // :<prefix> <numeric> <target> [parameters] :<trailing text>\r\n
    std::string format_reply(const std::string &prefix, const std::string &numeric, const std::string &target, const std::string &parameters, const std::string &trailing);
}
#endif