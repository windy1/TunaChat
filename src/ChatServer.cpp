//
// Created by Walker Crouse on 9/24/18.
//

#include "CliConn.h"
#include <memory>

using std::make_unique;
using std::thread;

/// thread function for new connections
int handleConnection(CliConn cli);

ChatServer::ChatServer(uint16_t port, int backlog, int bufferSize) :
    port(port),
    backlog(backlog),
    bufferSize(bufferSize) {}

ChatServer::ChatServer() : ChatServer(DEFAULT_PORT) {}

int ChatServer::start() {
    int socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (socketHandle == 0) {
        perror("Failed to create socket.");
        return ERR_SOCKET;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(socketHandle, (sockaddr*) &address, sizeof(address)) < 0) {
        perror("Failed to bind socket to port.");
        return ERR_BIND;
    }

    if (listen(socketHandle, backlog) < 0) {
        perror("Failed to start listening on socket.");
        return ERR_LISTEN;
    }

    printf("Accepting new connections...\n");

    int addrLen = sizeof(address);
    int clientSocket;
    vector<thread> threads;
    while ((clientSocket = accept(socketHandle, (sockaddr*) &address, (socklen_t*) &addrLen))) {
        threads.emplace_back(handleConnection, CliConn(*this, clientSocket));
    }

    for (auto &thread : threads) thread.join();

    return 0;
}

int handleConnection(CliConn cli) {
    // 1. Handshake
    if (!cli.verify()) {
        return cli.getError();
    }

    // 2. Authentication
    bool authenticated = false;
    while (!authenticated) {
        authenticated = cli.authenticate();
        if (cli.getError() != ERR_NONE) return cli.getError();
    }

    // 3. Request-Response
    while (cli.getError() == ERR_NONE) {
        cli.processCommand();
    }

    return cli.getError();
}

UserPtr ChatServer::authenticate(CliConn &conn, const string &user, const string &pwd) {
    connections.push_back(&conn);
    UserPtr usr = getUser(user);
    if (usr == nullptr) {
        usr = make_unique<User>(user);
        userList.push_back(usr);
    }
    return usr;
}

bool ChatServer::dispatch(const string &to, const string &from, const string &text) const {
    bool sent = false;
    for (auto &conn : connections) {
        if (conn->getUser()->getName() == to) {
            conn->sendMessage(from, text);
            sent = true;
        }
    }
    return sent;
}

void ChatServer::signOff(const string &user) {
    // TODO
}

int ChatServer::getPort() const {
    return port;
}

int ChatServer::getBacklog() const {
    return backlog;
}

int ChatServer::getBufferSize() const {
    return bufferSize;
}

const vector<UserPtr>& ChatServer::getUserList() const {
    return userList;
}

UserPtr ChatServer::getUser(const string &name) const {
    for (auto &user : userList) {
        if (user->getName() == name) return user;
    }
    return nullptr;
}

const vector<CliConn*>& ChatServer::getConnections() const {
    return connections;
}
