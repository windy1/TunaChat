//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_CLIENTCONNECTION_H
#define TUNACHAT_CLIENTCONNECTION_H


#include "ChatServer.h"

class ClientConnection {

    int socket;
    ChatServer &server;
    std::string lastMessage;
    int error = ERR_NONE;
    User *user;

    const std::string* readLine();

    void* close(std::string msg, int err);

    void writeUserList();

public:

    ClientConnection(ChatServer &server, int socket);

    int getSocket() const;

    const ChatServer& getServer() const;

    const std::string* getLastMessage() const;

    const User* getUser() const;

    int getError() const;

    bool verify();

    bool authenticate();

    void processCommand();

    void sendMessage(const std::string &from, const std::string &text);

};


#endif //TUNACHAT_CLIENTCONNECTION_H
