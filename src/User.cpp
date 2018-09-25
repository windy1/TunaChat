//
// Created by Walker Crouse on 9/24/18.
//

#include "User.h"
#include "CliConn.h"

User::User(ChatServer &server, const string &name) : server(server), name(name) {}

int User::sendMessage(const string &from, const string &text) {
    int sent = 0;
    for (auto &conn : getConnections()) {
        conn->sendMessage(from, text);
        sent++;
    }
    return sent;
}

ChatServer& User::getServer() const {
    return server;
}

const string& User::getName() const {
    return name;
}

const vector<CliConnPtr>& User::getConnections() const {
    connections.clear();
    for (auto &conn : server.getConnections()) {
        UserPtr user = conn->getUser();
        if (user != nullptr && user.get() == this) {
            connections.push_back(conn);
        }
    }
    return connections;
}

int User::numConnections() const {
    int count = 0;
    for (auto &conn : server.getConnections()) {
        if (conn->getUser().get() == this) {
            count++;
        }
    }
    return count;
}
