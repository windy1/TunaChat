//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_USER_H
#define TUNACHAT_USER_H

#include "ChatServer.h"
#include <string>

using std::string;

class User {

    ChatServer &server;
    string name;
    mutable vector<CliConnPtr> connections;

public:

    explicit User(ChatServer &server, const string &name);

    int sendMessage(const string &from, const string &text);

    ChatServer& getServer() const;

    const string& getName() const;

    int numConnections() const;

    const vector<CliConnPtr>& getConnections() const;

};


#endif //TUNACHAT_USER_H
