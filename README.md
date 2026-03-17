# ft_irc — IRC Server in C++98

An IRC server built from scratch in **C++98**, fully compatible with standard IRC clients like [irssi](https://irssi.org/). This project implements the core of the IRC protocol (inspired by [RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812)), handling real-time multi-client communication through non-blocking I/O and the `poll()` system call.

> A [42 School](https://42.fr/) project.

**Authors:** [Ayoub Diri](https://github.com/ayoubediri) · [Mohammed Akarkaou](https://github.com/makarkao) · [Youssef El Bouzidi](https://github.com/Yo-omega)

---

## Features

### Core IRC Protocol
- **Capability Handshake** — `CAP LS` support for modern IRC client negotiation
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

Where:
- `port` must be between `1024` and `65535`
- `password` must be `1..128` chars, without spaces/newlines

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
| `CAP`   | Capability negotiation (`CAP LS` implemented) |
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

## Implementation Notes (Current Behavior)

- **Registration requirement**: clients must authenticate with `PASS` before `NICK`/`USER` are accepted.
- **Connection is fully registered** only after both valid `NICK` and `USER` are set.
- **Nickname validation**: max length `16`, must start with a letter, then letters/digits/`-`/`_`.
- **Channel creation**: first `JOIN #channel` creates the channel and makes that user channel operator.
- **`JOIN` restrictions enforced**:
	- invite-only (`+i`)
	- user limit (`+l`)
	- key-protected channel (`+k`) via `JOIN #chan <key>`
- **`TOPIC` with `+t`**: only channel operators can change topic.
- **`MODE #channel`** with no extra args returns active channel modes.
- **Unknown commands** return standard IRC `ERR_UNKNOWNCOMMAND`.
- **Line handling**: both `\r\n` (IRC standard) and `\n` are accepted.

### MODE flags implemented

- `+i` / `-i` — invite-only channel
- `+t` / `-t` — only operators may set topic
- `+k <key>` / `-k` — channel key
- `+o <nick>` / `-o <nick>` — give/remove channel operator
- `+l <limit>` / `-l` — user limit

### Bot and CTCP/DCC behavior

- Built-in bot runs as **`CalcBot`** and responds to `!calc` in channels.
- CTCP/DCC payloads are relayed transparently through `PRIVMSG`, enabling client-side DCC workflows.

---

## Tested With

- **irssi** v1.4.5
- **netcat** (`nc`)
- Verified on **Linux** (Ubuntu/Debian)
