#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

namespace Utils
{
    std::string to_upper(const std::string &str);
    bool validate_nick(const std::string &nick);
};

#endif