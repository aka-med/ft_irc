# ft_irc — IRC Server in C++98

An IRC server built from scratch in **C++98**, fully compatible with standard IRC clients like [irssi](https://irssi.org/). This project implements the core of the IRC protocol (inspired by [RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812)), handling real-time multi-client communication through non-blocking I/O and the `poll()` system call.

> A [42 School](https://42.fr/) project.

**Authors:** [Youssef El Bouzidi](https://github.com/Yo-omega) · [Ayoub Diri](https://github.com/ayoubediri) · [Mohammed Akarkaou](https://github.com/makarkao)

---

## Features

### Core IRC Protocol
- **User Registration** — `PASS`, `NICK`, `USER` authentication flow
- **Channel Management** — `JOIN`, `PART`, `KICK`, `INVITE`, `TOPIC`
- **Messaging** — `PRIVMSG` for both channels and private DMs
- **Operator Privileges** — `MODE` with support for `+i`, `+t`, `+k`, `+o`, `+l`
- **Connection Handling** — `PING`/`PONG` keepalive, `QUIT`

### Networking
- Non-blocking sockets with `poll()` multiplexing
- Buffered, event-driven I/O (`POLLIN` / `POLLOUT`)
- Handles partial reads and multi-command buffers
- Compatible with both `\r\n` (IRC standard) and `\n` (netcat)

### Extended Features

**CalcBot 🤖** — A built-in channel bot that evaluates math expressions:
```
<user> !calc 42 + 13 - 5
<CalcBot> user, the answer to 42 + 13 - 5 is 50
```

**DCC File Transfer** — The server transparently relays CTCP/DCC messages between clients, enabling peer-to-peer file transfers through IRC clients like irssi.

---

## Quick Start

### Prerequisites
- A C++98-compatible compiler (e.g. `g++`, `clang++`)
- `make`
- An IRC client (`irssi` recommended) or `netcat`

### Build & Run
```bash
make
./ircserv <port> <password>
```

**Example:**
```bash
./ircserv 6667 mypassword
```

### Connect with irssi
```bash
irssi -c 127.0.0.1 -p 6667 -w mypassword
```

### Connect with netcat
```bash
nc localhost 6667
PASS mypassword
NICK mynick
USER mynick 0 * :My Name
JOIN #general
PRIVMSG #general :Hello, world!
```

---

## Project Structure

```
ft_irc/
├── includes/           # Header files
│   ├── Server.hpp      # Server class with poll() event loop
│   ├── Client.hpp      # Client state and buffered I/O
│   ├── Channel.hpp     # Channel membership and modes
│   ├── Bot.hpp         # CalcBot
│   ├── Parser.hpp      # IRC message parser
│   ├── Commands.hpp    # Command handler declarations
│   ├── Reply.hpp       # IRC numeric reply formatting
│   └── Utils.hpp       # String utilities
├── srcs/
│   ├── Server.cpp      # Socket setup, poll loop, client management
│   ├── Client.cpp      # Command routing and execution
│   ├── Channel.cpp     # Channel operations and broadcasting
│   ├── Bot.cpp         # Calculator bot logic
│   ├── Parser.cpp      # RFC-compliant message parsing
│   ├── Reply.cpp       # Reply/error code formatting
│   ├── Utils.cpp       # Helper functions
│   ├── main.cpp        # Entry point
│   └── Command/        # Individual command implementations
│       ├── Pass.cpp, Nick.cpp, User.cpp
│       ├── Join.cpp, Part.cpp, Kick.cpp
│       ├── Privmgs.cpp, Mode.cpp
│       ├── Ping.cpp, Quit.cpp
│       ├── invite.cpp, topic.cpp
│       └── Unknown.cpp
└── Makefile
```

---

## Supported IRC Commands

| Command | Description |
|---------|-------------|
| `PASS`  | Set connection password |
| `NICK`  | Set or change nickname |
| `USER`  | Complete registration |
| `JOIN`  | Join a channel |
| `PART`  | Leave a channel |
| `KICK`  | Remove a user from a channel |
| `INVITE`| Invite a user to a channel |
| `TOPIC` | View or set channel topic |
| `MODE`  | Set channel/user modes (`+i`, `+t`, `+k`, `+o`, `+l`) |
| `PRIVMSG`| Send a message to a channel or user |
| `PING`  | Connection keepalive |
| `QUIT`  | Disconnect from the server |

---

## Tested With

- **irssi** v1.4.5
- **netcat** (`nc`)
- Verified on **Linux** (Ubuntu/Debian)
