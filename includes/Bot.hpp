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
	std::vector<std::string>	_quotes;

	void		initQuotes();
	std::string	cmdHelp() const;
	std::string	cmdTime() const;
	std::string	cmdInfo() const;
	std::string	cmdHello(const std::string& sender) const;
	std::string	cmdQuote() const;
	std::string	cmdWho(const std::vector<std::string>& users) const;

public:
	Bot();
	Bot(const std::string& nickname);
	Bot(const Bot& other);
	Bot& operator=(const Bot& other);
	~Bot();

	std::string		processCommand(const std::string& sender, const std::string& message);
	std::string		onJoin(const std::string& nick, const std::string& channel);
	std::string		onPart(const std::string& nick, const std::string& channel);
	std::string		onQuit(const std::string& nick);
	std::string		onNickChange(const std::string& oldNick, const std::string& newNick);

	const std::string&	getNick() const;
	bool				isRunning() const;
	void				start();
	void				stop();
};

#endif
