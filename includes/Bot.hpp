#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iostream>

class Server;

class Bot
{
private:
	std::string					_nick;
	bool						_running;

public:
	Bot();
	Bot(const std::string& nickname);
	Bot(const Bot& other);
	Bot& operator=(const Bot& other);
	~Bot();

	std::string		processChannelMessage(const std::string& sender, const std::string& channel, const std::string& message);

	const std::string&	getNick() const;
	bool				isRunning() const;
	void				start();
	void				stop();
};

#endif
