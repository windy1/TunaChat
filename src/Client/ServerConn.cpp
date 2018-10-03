//
// Created by Walker Crouse on 9/26/18.
//

#include "ServerConn.h"
#include "Terminal/windows.h"
#include "Terminal/MainWindow.h"
#include "MessageChannel.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

using std::stringstream;
using std::make_shared;

///
/// == ServerConn ==
///

ServerConn::ServerConn(ChatClient &client, const string &host, int port, int bufferSize) :
    client(client), host(host), port(port), bufferSize(bufferSize) {
    status = init();
    status = sayHello();
}

///
/// == Methods ==
///

int ServerConn::authenticate(const string &user, const string &pwd) {
    char authStr[1024];
    sprintf(authStr, "%s:%s:%s\n", PROTO_AUTH, user.c_str(), pwd.c_str());
    string res;
    getResponse(string(authStr), res);

    client.getTerminal().getMainWindow()->debug(res);

    StatusPtr st = client.getTerminal().getStatusWindow();
    if (strcmp(res.c_str(), PROTO_AUTHYES) == 0) {
        authenticated = true;
        char str[500];
        sprintf(str, "Successfully authenticated as %s [%s]", user.c_str(), host.c_str());
        st->set(str);
        client.getTerminal().getInputWindow()->setTag(user);
        msgChan = make_shared<MessageChannel>(*this);
        this->user = user;
        return status = STATUS_OK;
    } else if (strcmp(res.c_str(), PROTO_AUTHNO) == 0) {
        st->error("Invalid username or password");
        return status = STATUS_INVALID_ARG;
    } else {
        st->error("An unknown server error occurred");
        return status = STATUS_BAD_RESPONSE;
    }
}

int ServerConn::sendMessage(const string &user, const string &text) {
    char req[1024];
    sprintf(req, "%s:%s:%s\n", PROTO_TO, user.c_str(), text.c_str());
    client.getTerminal().getMainWindow()->debug(req);
    send(socket, req, strlen(req), 0);
    return status = STATUS_OK;
}

int ServerConn::requestList() {
    char req[strlen(PROTO_LIST + 1)];
    sprintf(req, "%s\n", PROTO_LIST);
    send(socket, req, strlen(req), 0);
    return status = STATUS_OK;
}

int ServerConn::disconnect() {
    if (msgChan != nullptr) msgChan->shutdown();
    shutdown(socket, SHUT_RD);
    if (msgChan != nullptr) msgChan->getThread().join();
    msgChan = nullptr;
    if (authenticated) {
        char req[strlen(PROTO_BYE) + 1];
        sprintf(req, "%s\n", PROTO_BYE);
        send(socket, req, strlen(req), 0);
    }
    return status = STATUS_CLOSED;
}

///
/// == Getters ==
///

ChatClient& ServerConn::getClient() const {
    return client;
}

MsgChanPtr ServerConn::getMessageChannel() const {
    return msgChan;
}

bool ServerConn::isAuthenticated() const {
    return authenticated;
}

const string& ServerConn::getUser() const {
    return user;
}

const string& ServerConn::getHost() const {
    return host;
}

int ServerConn::getPort() const {
    return port;
}

int ServerConn::getSocket() const {
    return socket;
}

int ServerConn::getBufferSize() const {
    return bufferSize;
}

int ServerConn::getStatus() const {
    return status;
}

///
/// == Private methods ==
///

int ServerConn::init() {
    Terminal &term = client.getTerminal();
    StatusPtr statusWin = term.getStatusWindow();

    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        statusWin->error("Bad socket");
        return STATUS_BAD_SOCKET;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
        // try name lookup
        string ip;
        tuna::host2ip(host, ip);
        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
            statusWin->error("Invalid host name.");
            return STATUS_INVALID_ARG;
        }
    }

    if (::connect(socket, (sockaddr*) &addr, sizeof(addr)) < 0) {
        statusWin->error("Connection failed.");
        return STATUS_BAD_CONNECT;
    }

    statusWin->set("Connected; authenticate with /auth <user> <pass>");

    term.getInputWindow()->setTag(host);

    return STATUS_OK;
}

int ServerConn::sayHello() {
    string res;
    char hello[100];
    sprintf(hello, "%s\n", PROTO_HELLO);
    getResponse(hello, res);
    if (strcmp(res.c_str(), PROTO_HELLO) != 0) {
        client.getTerminal().getStatusWindow()->error("Connection refused.");
        return STATUS_BAD_RESPONSE;
    }
    return STATUS_OK;
}

void ServerConn::getResponse(const string &req, string &res) {
    send(socket, req.c_str(), req.size(), 0);
    char buffer[bufferSize];
    read(socket, buffer, (size_t) bufferSize);
    stringstream in(buffer);
    in >> res;
}
