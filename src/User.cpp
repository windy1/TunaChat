//
// Created by Walker Crouse on 9/24/18.
//

#include "User.h"
#include "CliConn.h"

User::User(ChatServer &server, const string &name) : server(server), name(name) {}

ChatServer& User::getServer() const {
    return server;
}

const string& User::getName() const {
    return name;
}

int User::connections() const {
    int count = 0;
    for (auto &conn : server.getConnections()) {
        if (conn->getUser().get() == this) {
            count++;
        }
    }
    return count;
}
