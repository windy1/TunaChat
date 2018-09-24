//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CHATSERVER_H
#define TUNACHAT_CHATSERVER_H

#define ERR_NONE_TERM   -1
#define ERR_NONE        0
#define ERR_SOCKET      1
#define ERR_BIND        2
#define ERR_LISTEN      3
#define ERR_NO_DATA     4
#define ERR_BAD_HS      5
#define ERR_AUTH_FORMAT 6
#define ERR_CMD_FORMAT  7

#define PROTO_HELLO     "HELLO"
#define PROTO_AUTHYES   "AUTHYES"
#define PROTO_AUTHNO    "AUTHNO"
#define PROTO_SIGNOFF   "SIGNOFF"

#define PROTO_HEAD_AUTH     "AUTH"
#define PROTO_HEAD_SIGNIN   "SIGNIN"

#define PROTO_CMD_LIST  "LIST"
#define PROTO_CMD_TO    "To"
#define PROTO_CMD_FROM  "From"
#define PROTO_CMD_BYE   "BYE"


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
