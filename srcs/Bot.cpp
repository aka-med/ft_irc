#include "../includes/Bot.hpp"

Bot::Bot(): _nick("IRCBot"), _running(false)
{
	initQuotes();
}

Bot::Bot(const std::string& nickname): _nick(nickname), _running(false)
{
	initQuotes();
}

Bot::Bot(const Bot& other): _nick(other._nick), _running(other._running),
_quotes(other._quotes)
{
}

Bot& Bot::operator=(const Bot& other)
{
	if (this != &other)
	{
		_nick = other._nick;
		_running = other._running;
		_quotes = other._quotes;
	}
	return (*this);
}

Bot::~Bot()
{
}

void Bot::initQuotes()
{
	_quotes.push_back("Talk is cheap. Show me the code. - Linus Torvalds");
	_quotes.push_back("First, solve the problem. Then, write the code.");
	_quotes.push_back("Code is like humor. When you have to explain it, its bad.");
	_quotes.push_back("Fix the cause, not the symptom. - Steve Maguire");
	_quotes.push_back("Simplicity is the soul of efficiency. - Austin Freeman");
	_quotes.push_back("Make it work, make it right, make it fast. - Kent Beck");
	_quotes.push_back("Any fool can write code that a computer can understand.");
	_quotes.push_back("The best error message is the one that never shows up.");
	std::srand(static_cast<unsigned int>(std::time(NULL)));
}

std::string Bot::processCommand(const std::string& sender, const std::string& message)
{
	if (!_running || message.empty() || message[0] != '!')
		return ("");

	std::string cmd;
	size_t sp = message.find(' ');
	if (sp != std::string::npos)
		cmd = message.substr(0, sp);
	else
		cmd = message;

	for (size_t i = 0; i < cmd.size(); i++)
	{
		if (cmd[i] >= 'A' && cmd[i] <= 'Z')
			cmd[i] = cmd[i] + 32;
	}

	if (cmd == "!help")
		return (cmdHelp());
	if (cmd == "!time")
		return (cmdTime());
	if (cmd == "!info")
		return (cmdInfo());
	if (cmd == "!hello")
		return (cmdHello(sender));
	if (cmd == "!quote")
		return (cmdQuote());
	return ("");
}

std::string Bot::processChannelMessage(const std::string& sender, const std::string& channel, const std::string& message)
{
	if (!_running || message.empty())
		return ("");

	if (message.find("!calc ") == 0)
	{
		std::string expr = message.substr(6);
		long result = 0;
		long currentNum = 0;
		char lastOp = '+';
		size_t i = 0;

		while (i < expr.length())
		{
			if (expr[i] == ' ')
			{
				i++;
				continue;
			}

			if (expr[i] >= '0' && expr[i] <= '9')
			{
				currentNum = 0;
				while (i < expr.length() && (expr[i] >= '0' && expr[i] <= '9'))
				{
					currentNum = currentNum * 10 + (expr[i] - '0');
					i++;
				}
				
				if (lastOp == '+')
					result += currentNum;
				else if (lastOp == '-')
					result -= currentNum;
			}
			else if (expr[i] == '+' || expr[i] == '-')
			{
				lastOp = expr[i];
				i++;
			}
			else
			{
				return ":" + _nick + " PRIVMSG " + channel + " :" + sender + ", I only understand digits, +, and -.\r\n";
			}
		}

		std::stringstream ss;
		ss << ":" << _nick << " PRIVMSG " << channel << " :" << sender << ", the answer to " << expr << " is " << result << "\r\n";
		return ss.str();
	}
	
	return ("");
}

// automatic events

std::string Bot::onJoin(const std::string& nick, const std::string& channel)
{
	if (!_running)
		return ("");
	return ("Welcome " + nick + " to " + channel + "! Type !help to see what I can do");
}

std::string Bot::onPart(const std::string& nick, const std::string& channel)
{
	if (!_running)
		return ("");
	return (nick + " just left " + channel + ". See you around!");
}

std::string Bot::onQuit(const std::string& nick)
{
	if (!_running)
		return ("");
	return ("Goodbye " + nick + "! Hope to see you again");
}

std::string Bot::onNickChange(const std::string& oldNick, const std::string& newNick)
{
	if (!_running)
		return ("");
	return (oldNick + " is now known as " + newNick);
}

// commands

std::string Bot::cmdHelp() const
{
	std::string h;
	h += "=== " + _nick + " ===\n";
	h += "  !help  - show commands\n";
	h += "  !time  - server time\n";
	h += "  !info  - server info\n";
	h += "  !hello - get a greeting\n";
	h += "  !quote - random quote";
	return (h);
}

std::string Bot::cmdTime() const
{
	std::time_t now = std::time(NULL);
	std::string t = std::ctime(&now);
	if (!t.empty() && t[t.size() - 1] == '\n')
		t.erase(t.size() - 1);
	return ("Server time: " + t);
}

std::string Bot::cmdInfo() const
{
	return ("ft_irc server v1.0 | C++98");
}

std::string Bot::cmdHello(const std::string& sender) const
{
	return ("Hey " + sender + "! Welcome to ft_irc");
}

std::string Bot::cmdQuote() const
{
	if (_quotes.empty())
		return ("No quotes");
	int idx = std::rand() % static_cast<int>(_quotes.size());
	return (_quotes[idx]);
}

const std::string& Bot::getNick() const
{
	return (_nick);
}

bool Bot::isRunning() const
{
	return (_running);
}

void Bot::start()
{
	_running = true;
}

void Bot::stop()
{
	_running = false;
}
