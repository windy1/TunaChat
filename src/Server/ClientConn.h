//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CLIENTCONNECTION_H
#define TUNACHAT_CLIENTCONNECTION_H

#include "ChatServer.h"
#include <memory>

using std::string;
using std::thread;

class ClientConn {

    static const string NAME_UNKNOWN;
    static int LAST_ID;

    ChatServer &server;
    UserPtr user;

    mutable string username;
    mutable string addrStr;
    mutable string tag;

    sockaddr_in address;
    int socket;
    int status = STATUS_OK;
    string lastMessage;
    int id;

    thread th;

    /**
     * Reads the next line from the client. This is a blocking operation; execution will not proceed until new data is
     * received or the connection is has been shutdown.
     */
    const string* readLine();

    /**
     * Writes the server's user list to the client.
     */
    void writeUserList();

public:

    ClientConn(ChatServer &server, sockaddr_in address, int socket);

    /**
     * Initiates the handshake protocol for the connection by reading an expected "HELLO" from the client and sends one
     * in return. If the client does not send the correct message, it will be closed immediately.
     *
     * @return true if successful, false if the connection has been closed
     */
    bool verify();

    /**
     * Authenticates the connection with the server and assigns this connection's User if successful. An unsuccessful
     * authentication attempt will not close the connection; however, an improperly formatted request will.
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

    /**
     * Signals to the client that they are being signed-off and shuts down the socket. This method sets the connections
     * status to STATUS_SHUTDOWN so that once recv() is no longer blocking in processCommand(), the connection knows not
     * to process any more data or listen on the socket any longer.
     *
     * This method may be called asynchronously. This method does not handle releasing resources after the connection
     * has been shutdown.
     */
    void shutdown();

    /**
     * Signals to the client the the specified User has signed-off.
     *
     * @param user that signed off
     */
    void goodbye(UserPtr user);

    /**
     * Forcibly closes the connection with the specified message and status code.
     *
     * @param msg status message
     * @param status code
     * @return nullptr
     */
    void* close(const string &msg, int status);

    /**
     * Returns the server associated with this connection.
     *
     * @return server instance
     */
    ChatServer& getServer() const;

    /**
     * Returns the User associated with this connection, if any.
     *
     * @return user associated with connection if this connection has been authenticated, nullptr otherwise
     */
    UserPtr getUser() const;

    /**
     * Returns a string identifier for this connection.
     *
     * @return string identifier
     */
    const string& getTag() const;

    /**
     * Returns a unique identifier for this connection.
     *
     * @return unique id
     */
    int getId() const;

    /**
     * Returns the address of this connection.
     *
     * @return address of connection
     */
    const sockaddr_in& getAddress() const;

    /**
     * Returns the client socket.
     *
     * @return client socket
     */
    int getSocket() const;

    /**
     * Returns the last message received on this connection.
     *
     * @return last message received
     */
    const string* getLastMessage() const;

    /**
     * Returns the current status code of the connection.
     *
     * @return status code
     */
    int getStatus() const;

    /**
     * Returns the thread instance that the connection is being handled in.
     *
     * @return connection thread
     */
    thread& getThread();

};


#endif //TUNACHAT_CLIENTCONNECTION_H
