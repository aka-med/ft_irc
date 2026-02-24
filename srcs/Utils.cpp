#include "../includes/Utils.hpp"

std::string Utils::to_upper(const std::string &str)
{
    std::string result = str;

    for (size_t i = 0; i < result.size(); ++i)
    {
        if (result[i] >= 'a' && result[i] <= 'z')
            result[i] = result[i] - ('a' - 'A');
    }
    return result;
}

bool Utils::validate_nick(const std::string &nick)
{
    if (nick.empty() || nick.size() > 16)
        return false;

    if (nick[0] < 'A' || (nick[0] > 'Z' && nick[0] < 'a') || nick[0] > 'z')
        return false;

    for (size_t i = 1; i < nick.size(); ++i)
    {
        char c = nick[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
              (c >= '0' && c <= '9') || c == '-' || c == '_'))
            return false;
    }
    return true;
}