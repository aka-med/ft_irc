#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Channel::Channel(std::string name) : _name(name), _topic(""), _t(0), _i(0), _k(0), _limit(0)
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

