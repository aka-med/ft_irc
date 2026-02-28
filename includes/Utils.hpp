#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace Utils
{
    std::string to_upper(const std::string &str);
    bool validate_nick(const std::string &nick);
    std::string join(const std::vector<std::string> &vec, size_t start, const std::string &delimiter);
    std::string int_to_string(int value);
};

#endif