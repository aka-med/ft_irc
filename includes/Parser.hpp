#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

class Server;

struct IRCMessage
{
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	params;

	bool empty() const { return (command.empty()); }
};

class Parser
{
private:
	Parser();

public:
	static IRCMessage	parseLine(const std::string& line);
	static int			validatePort(const std::string& portStr);
	static bool			validatePassword(const std::string& password);
	static std::string	toUpper(const std::string& str);
};

#endif
