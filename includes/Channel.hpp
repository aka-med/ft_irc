#ifndef CHANNEL_HPP
#define CHANNEL_HPP 

#include <string>
#include <vector>

class Client;

class Channel
{
    private:
        std::vector<Client> _users;
        std::string _name;
        std::string _topic;
        std::vector<Client> _operators;
        int _t;
        int _i;
        int _k;
        std::string _key;
        int _limit;
    public:
        Channel(std::string name);
        const std::string &get_name() const;
        const std::string &get_topic() const;
        void set_topic(const std::string &topic);
        void add_user(const Client &client);
        void remove_user(const Client &client);
        bool is_user_in_channel(const Client &client) const;
        void add_operator(const Client &client);
        void remove_operator(const Client &client);
        bool is_operator(const Client &client) const;
        void set_key(const std::string &key);
        bool check_key(const std::string &key) const;
        void set_limit(int limit);
        int get_limit() const;
};
#endif