//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CLIENTCONNECTION_H
#define TUNACHAT_CLIENTCONNECTION_H

#include "ChatServer.h"
#include <memory>

using std::string;

class CliConn {

    static const string NAME_UNKNOWN;
    static int LAST_ID;

    ChatServer &server;
    UserPtr user;
    mutable string username;

    sockaddr_in address;
    mutable string addrStr;
    mutable string tag;

    int socket;
    string lastMessage;
    int status = STATUS_OK;
    int id;

    /**
     * Reads the next line from the client. This is a blocking operation; execution will not proceed until new data is
     * received or the connection is closed.
     */
    const string* readLine();

    /// forcibly closes the connection with the specified error
    void* close(string msg, int status);

    /// writes the server's user list to the client
    void writeUserList();

public:

    CliConn(ChatServer &server, sockaddr_in address, int socket);

    /**
     * Initiates the handshake protocol for the connection by sending a message and awaits the proper reply.
     *
     * @return true if successful, false if the connection has been closed
     */
    bool verify();

    /**
     * Authenticates the connection with the server and assigns this connection's User if successful. An unsuccessful
     * authentication attempt will not close the connection.
     *
     * @return true if successful
     */
    bool authenticate();

    /**
     * Processes the next message as a command.
     */
    void processCommand();

    /**
     * Sends a message to the client from the specified User.
     *
     * @param from the user the message is from
     * @param text the text of the message
     */
    void sendMessage(const string &from, const string &text);

    /// kicks this connection from the server
    void kick();

    /// returns the parent server
    ChatServer& getServer() const;

    /// returns the User associated with this connection
    UserPtr getUser() const;

    const string& getUsername() const;

    const sockaddr_in& getAddress() const;

    const string& getAddressString() const;

    const string& getTag() const;

    /// returns the client socket
    int getSocket() const;

    /// returns the last message received on this connection
    const string* getLastMessage() const;

    /// returns the current status code of the connection
    int getStatus() const;

};


#endif //TUNACHAT_CLIENTCONNECTION_H
