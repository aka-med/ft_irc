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
        std::vector<Client> _invited_users;
        bool _t;
        bool _i;
        bool _k;
        bool _l;
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
        bool get_k_flag() const;
        bool get_i_flag() const;
        bool get_t_flag() const;
        bool get_l_flag() const;
        void set_k_flag(bool value);
        void set_i_flag(bool value);
        void set_t_flag(bool value);
        void set_l_flag(bool value);
        bool check_key(const std::string &key) const;
        void set_limit(int limit);
        int get_limit() const;
        size_t get_users_count() const;
        size_t get_operators_count() const;
        std::string get_users_list_names() const;
        void add_invited_user(const Client &client);
        void remove_invited_user(const Client &client);
        bool is_user_invited(const Client &client) const;
        void broadcast(const std::string &message, const Client &sender) const;
};
#endif