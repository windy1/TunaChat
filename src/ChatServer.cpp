//
// Created by Walker Crouse on 9/24/18.
//

#include "ClientConnection.h"

int handleConnection(ClientConnection cli);

ChatServer::ChatServer(uint16_t port, int backlog, int bufferSize) :
    port(port),
    backlog(backlog),
    bufferSize(bufferSize) {}

ChatServer::ChatServer() : ChatServer(DEFAULT_PORT) {}

int ChatServer::getPort() const {
    return port;
}

int ChatServer::getBacklog() const {
    return backlog;
}

int ChatServer::getBufferSize() const {
    return bufferSize;
}

const std::vector<User>& ChatServer::getUserList() const {
    return userList;
}

User* ChatServer::authenticate(ClientConnection &conn, const std::string &user, const std::string &pwd) {
    connections.push_back(&conn);
    printf("connections.size() = %d\n", (int) connections.size());
    for (auto &userObj : userList) {
        if (userObj.getName() == user) {
            return &userObj;
        }
    }
    return &userList.emplace_back(user);
}

void ChatServer::signOff(const std::string &user) {
    for (auto i = userList.begin(); i != userList.end(); i++) {
        if (i->getName() == user) {
            userList.erase(i);
            return;
        }
    }
}

bool ChatServer::dispatch(const std::string &to, const std::string &from, const std::string &text) const {
    bool sent = false;
    for (auto &conn : connections) {
        if (conn->getUser()->getName() == to) {
            conn->sendMessage(from, text);
            sent = true;
        }
    }
    return sent;
}

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
    std::vector<std::thread> threads;
    while ((clientSocket = accept(socketHandle, (sockaddr*) &address, (socklen_t*) &addrLen))) {
        threads.emplace_back(handleConnection, ClientConnection(*this, clientSocket));
    }

    for (auto &thread : threads) thread.join();

    return 0;
}

int handleConnection(ClientConnection cli) {
    if (!cli.verify()) return cli.getError();

    bool authenticated = false;
    while (!authenticated) {
        authenticated = cli.authenticate();
        if (cli.getError() != ERR_NONE) return cli.getError();
    }

    while (cli.getError() == ERR_NONE) cli.processCommand();

    return cli.getError();
}
