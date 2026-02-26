#include "../includes/Bot.hpp"

Bot::Bot(): _nick("IRCBot"), _running(false)
{
}

Bot::Bot(const std::string& nickname): _nick(nickname), _running(false)
{
}

Bot::Bot(const Bot& other): _nick(other._nick), _running(other._running)
{
}

Bot& Bot::operator=(const Bot& other)
{
	if (this != &other)
	{
		_nick = other._nick;
		_running = other._running;
	}
	return (*this);
}

Bot::~Bot()
{
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
