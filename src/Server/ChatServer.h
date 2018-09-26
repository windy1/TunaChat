//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CHATSERVER_H
#define TUNACHAT_CHATSERVER_H

#include "../tuna.h"
#include <cstdint>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstdio>
#include <sstream>
#include <vector>

using std::vector;
using std::string;

class User;
class ClientConn;

typedef std::shared_ptr<User> UserPtr;
typedef std::shared_ptr<ClientConn> ClientConnPtr;

class ChatServer {

    static uint16_t const DEFAULT_PORT = 12000;

    sockaddr_in address = {};
    uint16_t port;
    int socket;
    int backlog;
    int bufferSize;
    vector<UserPtr> userList;
    vector<ClientConnPtr> connections;
    int status;

    /**
     * Initializes the socket and starts listening on a new socket.
     *
     * @return status code
     */
    int init();

public:

    explicit ChatServer(uint16_t port, int backlog=10, int bufferSize=1024);

    ChatServer();

    /**
     * Starts the server. Initializes and starts listening on a new socket for new connections.
     *
     * @return status code
     */
    int start();

    /**
     * Authenticates the specified username and password. Creates a new User if not already online.
     *
     * @param user user that is authenticating
     * @param pwd user password
     * @return the resulting User
     */
    UserPtr authenticate(const string &user, const string &pwd);

    /**
     * Sends a message to each of the open connections for the specified User.
     *
     * @param to the User the message is for
     * @param from the User who sent the message
     * @param text the text of the message
     * @return the amount of connections the message was sent to
     */
    int dispatch(const string &to, const string &from, const string &text) const;

    /**
     * Spawns a detached thread to handle the disconnection and cleanup of all existing connections for the specified
     * user.
     *
     * @param user to sign off
     */
    void signOff(const string &user);

    /**
     * Removes all connection instances attached to this User and removed the User from the user list.
     *
     * @param user to remove
     * @return amount of connections removed
     */
    int cleanup(UserPtr user);

    /**
     * Returns all currently online users.
     *
     * @return users currently online
     */
    const vector<UserPtr>& getUserList() const;

    /**
     * Returns the user if currently online.
     *
     * @param name of user
     * @return User if online, nullptr otherwise
     */
    UserPtr getUser(const string &name) const;

    /**
     * Returns all current connections.
     *
     * @return current connections
     */
    const vector<ClientConnPtr>& getConnections() const;

    /**
     * Returns the address this server is running on.
     *
     * @return server address
     */
    const sockaddr_in& getAddress() const;

    /**
     * Returns the port number this server is running on.
     *
     * @return port number
     */
    int getPort() const;

    /**
     * Returns the socket that this server is listening on.
     *
     * @return socket number
     */
    int getSocket() const;

    /**
     * Returns the maximum amount of queued connections permitted.
     *
     * @return queued connections allowed
     */
    int getBacklog() const;

    /**
     * Returns the buffer size for incoming messages from clients
     *
     * @return data buffer size
     */
    int getBufferSize() const;

    /**
     * Returns the current status code of the server.
     *
     * @return status code
     */
    int getStatus() const;

};


#endif //TUNACHAT_CHATSERVER_H
