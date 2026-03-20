# Chat Application

A TCP-based chat server with separate client and user management tools.
Supports interactive messaging, user creation, and login/logout functionality.

## Project Structure

```
coursework-ChatApplication/
├── Makefile
├── make.sh
├── README.md
├── AGENTS.md
├── data/
│   └── users.txt
├── include/
│   ├── chat_config.h
│   └── server/
│       └── user.h
├── src/
│   ├── server.c
│   ├── users.c
│   └── client.c
├── build/
│   ├── server
│   └── client
└── tests/
    └── showip.c
```

## Demo on Another Machine

**Packaging (run from project root):**
```bash
zip server.zip src/server.c src/users.c include/server/user.h include/chat_config.h data/users.txt Makefile make.sh
zip client.zip src/client.c include/chat_config.h
```

**On the target machine:**

1. Unzip both archives into the same directory:
   ```bash
   unzip server.zip
   unzip client.zip
   ```

2. Build using the Makefile:
   ```bash
   make
   ```

3. Run the server from that directory (so `data/users.txt` resolves correctly):
   ```bash
   ./build/server
   ```

4. Run the client in a separate terminal:
   ```bash
   ./build/client
   ```

> **Note:** The server must be run from the directory containing `data/`.

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