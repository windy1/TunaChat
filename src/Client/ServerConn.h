//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_SERVERCONN_H
#define TUNACHAT_SERVERCONN_H

#include "ChatClient.h"

class ServerConn {

    ChatClient &client;
    string host;
    int port;
    int socket;
    int bufferSize;
    bool authenticated = false;
    int status = STATUS_CLOSED;

    int init();

    int sayHello();

    void getResponse(const string &req, string &res);

public:

    ServerConn(ChatClient &client, const string &host, int port, int bufferSize=1024);

    int authenticate(const string &user, const string &pwd);

    int sendMessage(const string &user, const string &msg);

    ChatClient& getClient() const;

    const string& getHost() const;

    int getPort() const;

    int getSocket() const;

    int getBufferSize() const;

    bool isAuthenticated() const;

    int getStatus() const;

};

#endif //TUNACHAT_SERVERCONN_H
