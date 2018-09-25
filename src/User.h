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

public:

    explicit User(ChatServer &server, const string &name);

    ChatServer& getServer() const;

    const string& getName() const;

    int connections() const;

};


#endif //TUNACHAT_USER_H
