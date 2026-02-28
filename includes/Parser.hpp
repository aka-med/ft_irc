#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

struct IRCMessage
{
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	params;

	bool empty() const { return (command.empty()); }
};

namespace Parser
{
	IRCMessage	parseLine(const std::string& line);
	int			validatePort(const std::string& portStr);
	bool			validatePassword(const std::string& password);
};

#endif
