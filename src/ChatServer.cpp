//
// Created by Walker Crouse on 9/24/18.
//

#include "CliConn.h"
#include "User.h"
#include <memory>

using std::make_shared;
using std::thread;
using std::remove;

/// thread function for new connections
int handleConnection(CliConnPtr cli);

ChatServer::ChatServer(uint16_t port, int backlog, int bufferSize) :
    port(port),
    backlog(backlog),
    bufferSize(bufferSize) {}

ChatServer::ChatServer() : ChatServer(DEFAULT_PORT) {}

int ChatServer::start() {
    printf("Starting server...\n");
    printf("  Port: %d\n", port);
    printf("  Socket: %d\n", socket);
    printf("  Backlog: %d\n", backlog);
    printf("  Buffer size: %d\n", bufferSize);

    if (init() != STATUS_OK) {
        return status;
    }

    printf("Accepting new connections...\n");


    sockaddr_in cliAddr = {};
    int addrLen = sizeof(sockaddr_in);
    int clientSocket;
    vector<thread> threads;
    while ((clientSocket = accept(socket, (sockaddr*) &cliAddr, (socklen_t*) &addrLen))) {
        CliConnPtr conn = make_shared<CliConn>(*this, cliAddr, clientSocket);
        connections.push_back(conn);
        threads.emplace_back(handleConnection, conn);
    }

    for (auto &thread : threads) thread.join();

    return 0;
}

int handleConnection(CliConnPtr cli) {
    printf("CONNECTION [%s]\n", cli->getTag().c_str());

    // 1. Handshake
    if (!cli->verify()) {
        return cli->getStatus();
    }

    printf("VERIFIED [%s]\n", cli->getTag().c_str());

    // 2. Authentication
    bool authenticated = false;
    while (!authenticated) {
        authenticated = cli->authenticate();
        if (cli->getStatus() != STATUS_OK) return cli->getStatus();
    }

    printf("AUTHENTICATED [%s]\n", cli->getTag().c_str());

    // 3. Request-Response
    while (cli->getStatus() == STATUS_OK) {
        cli->processCommand();
    }

    return cli->getStatus();
}

UserPtr ChatServer::authenticate(const string &user, const string &pwd) {
    UserPtr usr = getUser(user);
    if (usr == nullptr) {
        usr = make_shared<User>(*this, user);
        userList.push_back(usr);
    }
    return usr;
}

int ChatServer::dispatch(const string &to, const string &from, const string &text) const {
    int sent = 0;
    for (auto &conn : connections) {
        if (conn->getUser()->getName() == to) {
            conn->sendMessage(from, text);
            sent++;
        }
    }

    printf("DISPATCH\n");
    printf("  Text: %s\n", text.c_str());
    printf("  From: %s\n", from.c_str());
    printf("  To: %s\n", to.c_str());
    printf("  Sent: %d\n", sent);

    return sent;
}

void ChatServer::onConnectionClosed(CliConn *conn) {
    UserPtr user = conn->getUser();

    for (auto iter = connections.begin(); iter != connections.end(); iter++) {
        if ((*iter).get() == conn) {
            connections.erase(iter);
            break;
        }
    }

    if (user != nullptr && user->connections() == 0) {
        userList.erase(remove(userList.begin(), userList.end(), user), userList.end());
    }

    printf("CONNECTIONS:%d\n", (int) connections.size());
    printf("USERS:%d\n", (int) userList.size());
}

void ChatServer::signOff(const string &user) {
    printf("SIGNOFF [%s]\n", user.c_str());
    for (auto &conn : connections) {
        if (conn->getUser()->getName() == user) {
            conn->kick();
        }
    }
}

const sockaddr_in& ChatServer::getAddress() const {
    return address;
}

int ChatServer::getPort() const {
    return port;
}

int ChatServer::getSocket() const {
    return socket;
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

const vector<CliConnPtr>& ChatServer::getConnections() const {
    return connections;
}

int ChatServer::getStatus() const {
    return status;
}

int ChatServer::init() {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    printf("Opening socket...\n");

    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket == 0) {
        perror("Failed to open socket");
        return status = STATUS_BAD_SOCKET;
    }

    printf("Binding socket...\n");

    if (bind(socket, (sockaddr*) &address, sizeof(address)) < 0) {
        perror("Failed to bind socket to port");
        return status = STATUS_BAD_BIND;
    }

    if (listen(socket, backlog) < 0) {
        perror("Failed to start listening on socket.");
        return status = STATUS_BAD_LISTEN;
    }

    return status = STATUS_OK;
}
