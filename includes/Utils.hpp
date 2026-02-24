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
    std::string join(const std::vector<std::string> &vec, size_t start, const std::string &delimiter);
};

#endif