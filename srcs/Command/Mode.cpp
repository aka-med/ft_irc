#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

mode_data_s::mode_data_s(Server &server, Client *client, Channel &channel, const std::vector<std::string> &args) : 
 server(server), client(client), channel(channel), args(args), track_flags(""), track_params(""), params_index(3), sign('+'), prev_sign('0')
{

}

///////////////////////////////////---- util functions -----//////////////////////////////////////////

int validate_limit(std::string &limit_str)
{
    if(limit_str.empty())
        return -1;
    int len = 0;
    bool start_count = false;
    for(size_t i = 0; i < limit_str.length(); i++)
    {
        if(limit_str[i] != '0' && !start_count)
            start_count = true;
        if(!std::isdigit(limit_str[i]))
            return -1;
        if(start_count)
            len++;
    }
    if(len > 10)
        return -1;
    long limit = atol(limit_str.c_str());
    long max_int = std::numeric_limits<int>::max();
    if(limit > max_int || limit == 0)
        return -1;
    return limit;
}

bool validate_key(std::string key)
{
    for(size_t i = 0; i < key.length(); i++)
    {
        if(key[i] == ' ')
            return false;
    }
    return true;
}

void show_active_mods(Server &server, Client *client, Channel *channel)
{
    std::string modes = "+";
    std::string params = "";
    std::string reply;

    if (channel->get_i_flag())
            modes += "i";
    if (channel->get_t_flag())
        modes += "t";
    if (channel->get_k_flag())
    {
        modes += "k";
        params += " *";
    }
    if (channel->get_l_flag())
    {
        modes += "l";
        params += " " + Utils::int_to_string(channel->get_limit());
    }
    if (modes == "+")
        modes = "";
    std::string mode_params = channel->get_name() + " " + modes + params;
    reply = Reply::format_reply(server.get_name(), Reply::RPL_CHANNELMODEIS, client->get_nick(), mode_params, "");
    client->send(reply);
}

void track_flags_func(mode_data_t &mode_dt, const std::string &str)
{
    if(mode_dt.prev_sign != mode_dt.sign)
    {
        mode_dt.track_flags += mode_dt.sign;
        mode_dt.prev_sign = mode_dt.sign;
    }
    mode_dt.track_flags += str;
}

void track_flags_func(mode_data_t &mode_dt, std::string &str)
{
    if(!mode_dt.track_params.empty())
        mode_dt.track_params += " ";
    mode_dt.track_params += str;
}

void broadcast_changes(mode_data_t &mode_dt)
{
    std::string broadcast_params = mode_dt.channel.get_name() + " " + mode_dt.track_flags;
    if (!mode_dt.track_params.empty())
        broadcast_params += " " + mode_dt.track_params;

    std::string mode_msg = Reply::format_command(mode_dt.client, "MODE", broadcast_params, "");

    mode_dt.channel.broadcast(mode_msg, mode_dt.client);
    mode_dt.client->send(mode_msg);
}
///////////////////////////////////---- mode i function-----//////////////////////////////////////////


void mode_i_flag(mode_data_t &mode_dt)
{
    if(mode_dt.sign == '+' && !mode_dt.channel.get_i_flag())
        mode_dt.channel.set_i_flag(true);
    else if(mode_dt.sign == '-' && mode_dt.channel.get_i_flag())
        mode_dt.channel.set_i_flag(false);
    else
        return;
    track_flags_func(mode_dt, "i");
}


///////////////////////////////////---- mode t function-----//////////////////////////////////////////


void mode_t_flag(mode_data_t &mode_dt)
{
    if(mode_dt.sign == '+' && !mode_dt.channel.get_t_flag())
        mode_dt.channel.set_t_flag(true);
    else if(mode_dt.sign == '-' && mode_dt.channel.get_t_flag())
        mode_dt.channel.set_t_flag(false);
    else
        return;
    track_flags_func(mode_dt, "t");
}

///////////////////////////////////---- mode k function-----//////////////////////////////////////////

void mode_k_flag(mode_data_t &mode_dt)
{
    std::string reply;

    if(mode_dt.sign == '-' && mode_dt.channel.get_k_flag())
    {
        mode_dt.channel.set_k_flag(false);
        mode_dt.channel.set_key("");
    }
    else if (mode_dt.sign == '+' && !mode_dt.channel.get_k_flag())
    {
        if(mode_dt.args.size() <= mode_dt.params_index)
        {
            reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_NEEDMOREPARAMS, mode_dt.client->get_nick(), "MODE", "Not enough parameters");
            mode_dt.client->send(reply);
            return;
        }
        std::string key = mode_dt.args[mode_dt.params_index++];
        if(!validate_key(key))
        {
            reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_INVALIDMODEPARM, mode_dt.client->get_nick(), key, "Invalid mode parameter.");
            mode_dt.client->send(reply);
            return;
        }
        mode_dt.channel.set_k_flag(true);
        mode_dt.channel.set_key(key);
        track_flags_func(mode_dt, key);
    }
    else if (mode_dt.sign == '+' && mode_dt.channel.get_k_flag())
    {
        reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_KEYSET, mode_dt.client->get_nick(), mode_dt.channel.get_name(), "Channel key already set");
        mode_dt.client->send(reply);
        return;
    }
    else
        return;
    track_flags_func(mode_dt, "k");
}


///////////////////////////////////---- mode o function-----//////////////////////////////////////////


void mode_o_flag(mode_data_t &mode_dt)
{
    std::string reply;

    if(mode_dt.args.size() <= mode_dt.params_index)
    {
        reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_NEEDMOREPARAMS, mode_dt.client->get_nick(), "MODE", "Not enough parameters");
        mode_dt.client->send(reply);
        return;
    }
    std::string nick = mode_dt.args[mode_dt.params_index++];
    if(nick.empty())
    {
        reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_NOSUCHNICK, mode_dt.client->get_nick(), nick, "No such nick");
        mode_dt.client->send(reply);
        return;
    }
    Client *target_client_ptr = mode_dt.server.getClientByNick(nick);
    if(!target_client_ptr)
    {
        reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_NOSUCHNICK, mode_dt.client->get_nick(), nick, "No such nick");
        mode_dt.client->send(reply);
        return;
    }
    if(!mode_dt.channel.is_user_in_channel(target_client_ptr))
    {
        reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_USERNOTINCHANNEL, mode_dt.client->get_nick(), nick + " " + mode_dt.channel.get_name(), "They aren't on that channel");
        mode_dt.client->send(reply);
        return;
    }

    if(mode_dt.sign == '+' && !mode_dt.channel.is_operator(target_client_ptr))
        mode_dt.channel.add_operator(target_client_ptr);
    else if (mode_dt.sign ==  '-' && mode_dt.channel.is_operator(target_client_ptr))
        mode_dt.channel.remove_operator(target_client_ptr);
    else
        return;
    track_flags_func(mode_dt, nick);
    track_flags_func(mode_dt, "o");
}

///////////////////////////////////---- mode l function-----//////////////////////////////////////////



void mode_l_flag(mode_data_t &mode_dt)
{
    std::string reply;

    if(mode_dt.sign == '-' && mode_dt.channel.get_l_flag())
    {
        mode_dt.channel.set_l_flag(false);
        mode_dt.channel.set_limit(-1);
    }
    else if (mode_dt.sign == '+')
    {
        if(mode_dt.args.size() <= mode_dt.params_index)
        {
            reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_NEEDMOREPARAMS, mode_dt.client->get_nick(), "MODE", "Not enough parameters");
            mode_dt.client->send(reply);
            return;
        }
        std::string limit_str = mode_dt.args[mode_dt.params_index++];
        int limit = validate_limit(limit_str);
        if(limit < 0)
        {
            reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_INVALIDMODEPARM, mode_dt.client->get_nick(), limit_str, "Invalid mode parameter.");
            mode_dt.client->send(reply);
            return;
        }
        mode_dt.channel.set_limit(limit);
        mode_dt.channel.set_l_flag(true);
        track_flags_func(mode_dt, limit_str);
    }
    else
        return;
    track_flags_func(mode_dt, "l");
}




///////////////////////////////////----mode setter function-----//////////////////////////////////////////


void set_mods(Server &server, Client *client, const std::vector<std::string> &args, Channel &channel)
{

    mode_data_t mode_dt(server, client, channel, args);
    std::string &str_flags = const_cast<std::string &>(args[2]);
    std::string reply;

    for(size_t i = 0; i < str_flags.size(); i++)
    {
        if(str_flags[i] == '+' || str_flags[i] == '-')
        {
            if(mode_dt.sign != str_flags[i])
                mode_dt.sign = str_flags[i];
        }
        else if(str_flags[i] == 'i')
            mode_i_flag(mode_dt);
        else if (str_flags[i] == 't')
            mode_t_flag(mode_dt);
        else if (str_flags[i] == 'k')
            mode_k_flag(mode_dt);
        else if (str_flags[i] == 'o')
            mode_o_flag(mode_dt);
        else if(str_flags[i] == 'l')
            mode_l_flag(mode_dt);
        else
        {
            std::string str_char = "";
            str_char += str_flags[i];
            reply = Reply::format_reply(mode_dt.server.get_name(), Reply::ERR_UNKNOWNMODE, mode_dt.client->get_nick(), str_char, "is unknown mode char to me");
            mode_dt.client->send(reply);
        }
    }

    if (!mode_dt.track_flags.empty())
        broadcast_changes(mode_dt);
}

///////////////////////////////////----main mode function-----//////////////////////////////////////////
bool Command::mode(Server &server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if (args.size() < 2)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "MODE", "Not enough parameters");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (args[1].empty() || args[1][0] == '#')
    {
        std::string channel_name = args[1];
        if (!server.channelExists(channel_name))
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_NOSUCHCHANNEL, target, channel_name, "No such channel");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        Channel *channel = server.getChannel(channel_name);
        if (!channel->is_user_in_channel(client))
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_NOTONCHANNEL, target, channel_name, "You're not on that channel");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        if (args.size() == 2)
        {
            show_active_mods(server, client, channel);
            return Client::KEEP_CLIENT_SOCKET;
        }
        if (!channel->is_operator(client))
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_CHANOPRIVSNEEDED, target, channel_name, "You're not channel operator");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        set_mods(server, client, args, *channel);
        return Client::KEEP_CLIENT_SOCKET;
    }
    else
    {
        reply = ":" + client->get_nick() + " MODE " + client->get_nick() + " :" + args[1] + "\r\n";
        client->send(reply);
    }
    return Client::KEEP_CLIENT_SOCKET;
}
