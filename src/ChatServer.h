//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CHATSERVER_H
#define TUNACHAT_CHATSERVER_H


#include "tuna.h"
#include "User.h"
#include <cstdint>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <sstream>
#include <vector>

class ClientConnection;

class ChatServer {

    static uint16_t const DEFAULT_PORT = 12000;

    uint16_t port;
    int backlog;
    int bufferSize;
    std::vector<User> userList;
    std::vector<ClientConnection*> connections;

public:

    explicit ChatServer(uint16_t port, int backlog=10, int bufferSize=1024);

    ChatServer();

    int start();

    int getPort() const;

    int getBacklog() const;

    int getBufferSize() const;

    const std::vector<User>& getUserList() const;

    User* authenticate(ClientConnection &conn, const std::string &user, const std::string &pwd);

    void signOff(const std::string &user);

    bool dispatch(const std::string &to, const std::string &from, const std::string &text) const;

};


#endif //TUNACHAT_CHATSERVER_H
