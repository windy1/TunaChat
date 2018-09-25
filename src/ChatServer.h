//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CHATSERVER_H
#define TUNACHAT_CHATSERVER_H

#include "tuna.h"
#include <cstdint>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <sstream>
#include <vector>

using std::vector;
using std::string;

class User;
class CliConn;

typedef std::shared_ptr<User> UserPtr;
typedef std::shared_ptr<CliConn> CliConnPtr;

class ChatServer {

    static uint16_t const DEFAULT_PORT = 12000;

    sockaddr_in address = {};
    uint16_t port;
    int socket;
    int backlog;
    int bufferSize;
    vector<UserPtr> userList;
    vector<CliConnPtr> connections;
    int status;

    int init();

public:

    explicit ChatServer(uint16_t port, int backlog=10, int bufferSize=1024);

    ChatServer();

    /// starts the server and begins listening for incoming connections
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

    void signOff(const string &user);

    int cleanup(UserPtr user);

    const sockaddr_in& getAddress() const;

    /// returns the port this server is running on
    int getPort() const;

    int getSocket() const;

    /// returns the maximum amount of queued connections allowed
    int getBacklog() const;

    /// returns the buffer size for incoming messages from clients
    int getBufferSize() const;

    /// returns currently online users
    const vector<UserPtr>& getUserList() const;

    /// returns the user if currently online
    UserPtr getUser(const string &name) const;

    /// returns current connections
    const vector<CliConnPtr>& getConnections() const;

    int getStatus() const;

};


#endif //TUNACHAT_CHATSERVER_H
