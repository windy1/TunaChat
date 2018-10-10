TunaChat
========

Programming assignment #1 for Prof. Christian Skalka.  Included are both client and (partial) server implementations of
the UVMPM protocol. I've included both in my submission because there is some overlap in files they use, but only the 
client should be considered "complete". After the provided server was made available, I practically abandoned the 
server and only tested on the live server provided.

Building
--------

This project was compiled on macOS 10.12.6 (Sierra) with CMake and CLion for C++17. The project also links the Curses
library which is required for the client.

Running
-------

Start the client with `./TunaChat --client`. The client should be executed one-down from the root directory of the 
project due to relative file paths. This application must be run in Terminal; do not run in an IDE.

Usage
-----

Type `/help` for a full list of commands. You can use `/connect [host] [port]` to connect to a server and 
`/auth [user] [pass]` to authenticate. The arguments for both of these commands are optional and have their defaults
defined in `files/conf.txt`. You can modify user preferences with `/pref <key> <value>` and list them with `/prefs`.
