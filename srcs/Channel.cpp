#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Channel::Channel(std::string name) : _name(name), _topic(""), _t(false), _i(false), _k(false), _l(false), _key(""), _limit(0)
{

}

const std::string &Channel::get_name() const
{
    return _name;
}

const std::string &Channel::get_topic() const
{
    return _topic;
}

void Channel::set_topic(const std::string &topic)
{
    _topic = topic;
}

void Channel::add_user(const Client &client)
{
    _users.push_back(client);
}

void Channel::remove_user(const Client &client)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].get_fd() == client.get_fd())
        {
            _users.erase(_users.begin() + i);
            break;
        }
    }
}

bool Channel::is_user_in_channel(const Client &client) const
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].get_fd() == client.get_fd())
            return true;
    }
    return false;
}

bool Channel::is_operator(const Client &client) const
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i].get_fd() == client.get_fd())
            return true;
    }
    return false;
}

void Channel::add_operator(const Client &client)
{
    if (!is_operator(client))
        _operators.push_back(client);
}

void Channel::remove_operator(const Client &client)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i].get_fd() == client.get_fd())
        {
            _operators.erase(_operators.begin() + i);
            break;
        }
    }
}

void Channel::set_key(const std::string &key)
{
    _key = key;
}

bool Channel::check_key(const std::string &key) const
{
    return _key == key;
}

void Channel::set_limit(int limit)
{
    _limit = limit;
}

int Channel::get_limit() const
{
    return _limit;
}

std::string Channel::get_users_list_names() const
{
    std::string users_list;
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (is_operator(_users[i]))
            users_list += "@" + _users[i].get_nick() + " ";
        else
            users_list += _users[i].get_nick() + " ";
        if (i == _users.size() - 1)
            users_list.erase(users_list.size() - 1); 
    }
    return users_list;
}

size_t Channel::get_users_count() const
{
    return _users.size();
}

size_t Channel::get_operators_count() const
{
    return _operators.size();
}


void Channel::set_k_flag(bool value)
{
    _k = value;
}

void Channel::set_i_flag(bool value)
{
    _i = value;
}

void Channel::set_t_flag(bool value)
{
    _t = value;
}

bool Channel::get_k_flag() const
{
    return _k;
}

bool Channel::get_i_flag() const
{
    return _i;
}

bool Channel::get_t_flag() const
{
    return _t;
}

void Channel::set_l_flag(bool value)
{
    _l = value;
}

bool Channel::get_l_flag() const
{
    return _l;
}

void Channel::broadcast(const std::string &message, const Client &sender) const
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i].get_fd() != sender.get_fd())
            _users[i].send(message);
    }
}

void Channel::add_invited_user(const Client &client)
{
    _invited_users.push_back(client);
}

void Channel::remove_invited_user(const Client &client)
{
    for (size_t i = 0; i < _invited_users.size(); i++)
    {
        if (_invited_users[i].get_fd() == client.get_fd())
        {
            _invited_users.erase(_invited_users.begin() + i);
            break;
        }
    }
}

bool Channel::is_user_invited(const Client &client) const
{
    for (size_t i = 0; i < _invited_users.size(); i++)
    {
        if (_invited_users[i].get_fd() == client.get_fd())
            return true;
    }
    return false;
}