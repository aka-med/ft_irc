#include "../includes/Parser.hpp"
#include "../includes/Utils.hpp"

IRCMessage Parser::parseLine(const std::string& line)
{
	IRCMessage	msg;
	std::string	remaining = line;

	while (!remaining.empty() && remaining[0] == ' ')
		remaining.erase(0, 1);
	while (!remaining.empty() && remaining[remaining.size() - 1] == ' ')
		remaining.erase(remaining.size() - 1);
	if (remaining.empty())
		return (msg);

	if (remaining[0] == ':')
	{
		remaining.erase(0, 1);
		size_t sp = remaining.find(' ');
		if (sp == std::string::npos)
		{
			msg.prefix = remaining;
			return (msg);
		}
		msg.prefix = remaining.substr(0, sp);
		remaining = remaining.substr(sp + 1);
		while (!remaining.empty() && remaining[0] == ' ')
			remaining.erase(0, 1);
	}

	size_t sp = remaining.find(' ');
	if (sp == std::string::npos)
	{
		msg.command = Utils::to_upper(remaining);
		return (msg);
	}
	msg.command = Utils::to_upper(remaining.substr(0, sp));
	remaining = remaining.substr(sp + 1);
	while (!remaining.empty() && remaining[0] == ' ')
		remaining.erase(0, 1);

	while (!remaining.empty())
	{
		if (remaining[0] == ':')
		{
			msg.params.push_back(remaining.substr(1));
			break ;
		}
		sp = remaining.find(' ');
		if (sp == std::string::npos)
		{
			msg.params.push_back(remaining);
			break ;
		}
		msg.params.push_back(remaining.substr(0, sp));
		remaining = remaining.substr(sp + 1);
		while (!remaining.empty() && remaining[0] == ' ')
			remaining.erase(0, 1);
	}
	return (msg);
}

int Parser::validatePort(const std::string& portStr)
{
	if (portStr.empty())
		return (-1);
	for (size_t i = 0; i < portStr.size(); i++)
	{
		if (portStr[i] < '0' || portStr[i] > '9')
			return (-1);
	}
	if (portStr.size() > 5)
		return (-1);
	long port = std::atol(portStr.c_str());
	if (port < 1024 || port > 65535)
		return (-1);
	return (static_cast<int>(port));
}

bool Parser::validatePassword(const std::string& password)
{
	if (password.empty() || password.size() > 128)
		return (false);
	for (size_t i = 0; i < password.size(); i++)
	{
		char c = password[i];
		if (c == ' ' || c == '\0' || c == '\r' || c == '\n')
			return (false);
	}
	return (true);
}
