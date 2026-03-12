# Chat Application

A TCP-based chat server with separate client and user management tools.
Supports interactive messaging, user creation, and login/logout functionality.

## Project Structure

```
coursework-ChatApplication/
├── Makefile
├── README.md
├── AGENTS.md
├── include/
│   ├── chat_config.h
│   └── server/
│       └── user.h
├── src/
│   ├── server.c
│   ├── users.c
│   ├── client.c
│   └── newuser.c
├── build/
│   ├── server
│   ├── client
│   └── newuser
└── tests/
    └── showip.c
```

## Usage Example

**Client Output:**
```bash
My chat room client. Version One.
NEWUSER Mike Mike11
New user account created. Please login.
NEWUSER Mike Mike11
Denied. User account already exists.
SEND
Denied. Please login first.
LOGIN Tom Tom12
Denied. User name or password incorrect.
LOGIN Tom Tom11
login confirmed
SEND Hello, is anybody out there?
Tom: Hello, is anybody out there?
SEND Bye for now
Tom: Bye for now
LOGOUT
Tom left.
```

**Server Output:**
```bash
My chat room server. Version One.
New user account created
Tom login.
Tom: Hello, is anybody out there?
Tom: Bye for now
Tom logout
```

## Architecture

```
Client                          Server
  |                               |
  |---- "LOGIN Tom Tom11" ------->|
  |                               |
  |  parse command                |
  |  invoke login_user("Tom", "Tom11")
  |  send response back           |
  |<------ "login confirmed" -----|
  |                               |
```