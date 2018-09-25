//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CLIENTCONNECTION_H
#define TUNACHAT_CLIENTCONNECTION_H

#include "ChatServer.h"
#include <memory>

using std::string;

class CliConn {

    int socket;
    ChatServer &server;
    string lastMessage;
    int error = ERR_NONE;
    UserPtr user;

    /// reads the next line from the client
    const string* readLine();

    /// forcibly closes the connection with the specified error
    void* close(string msg, int err);

    /// writes the server's user list to the client
    void writeUserList();

public:

    CliConn(ChatServer &server, int socket);

    /// initiates the handshake protocol for the connection
    bool verify();

    /// attempts to authenticate the client with the server
    bool authenticate();

    /// processes an authenticated connection
    void processCommand();

    /// sends a message to the client from the specified User
    void sendMessage(const string &from, const string &text);

    /// kicks this connection from the server
    void kick();

    /// returns the parent server
    ChatServer& getServer() const;

    /// returns the User associated with this connection
    UserPtr getUser() const;

    /// returns the client socket
    int getSocket() const;

    /// returns the last message received on this connection
    const string* getLastMessage() const;

    /// returns the current error code of the connection
    int getError() const;

};


#endif //TUNACHAT_CLIENTCONNECTION_H
