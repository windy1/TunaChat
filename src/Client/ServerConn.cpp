//
// Created by Walker Crouse on 9/26/18.
//

#include "ServerConn.h"
#include "Terminal/windows.h"
#include "Terminal/MainWindow.h"
#include "MessageChannel.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>

using std::stringstream;
using std::make_shared;

///
/// == ServerConn ==
///

ServerConn::ServerConn(ChatClient &client, const string &host, int port, int bufferSize, int timeout) :
    client(client), host(host), port(port), bufferSize(bufferSize) {
    this->timeout = timeout;
    status = init();
    if (status == STATUS_OK) status = sayHello();
}

///
/// == Methods ==
///

int ServerConn::authenticate(const string &user, const string &pwd) {
    char authStr[1024];
    sprintf(authStr, "%s:%s:%s\n", PROTO_AUTH, user.c_str(), pwd.c_str());
    string res;
    getResponse(string(authStr), res);

    //client.getTerminal().getMainWindow()->debug(res);

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

int ServerConn::getTimeout() const {
    return timeout;
}

int ServerConn::getStatus() const {
    return status;
}

///
/// == Initialization ==
///

int ServerConn::init() {
    Terminal &term = client.getTerminal();
    StatusPtr st = term.getStatusWindow();

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (!openSocket()) {
        st->error("Bad socket");
        return STATUS_BAD_SOCKET;
    }
    if (!verifyHost(addr)) {
        st->error("Invalid host name.");
        return STATUS_INVALID_ARG;
    }
    if (!setBlocking(false)) {
        st->error("An unexpected error occurred: " + string(strerror(errno)));
        return STATUS_BAD_CONNECT;
    }

    int connSt = connect(addr);
    switch (connSt) {
        case STATUS_BAD_CONNECT:
            st->error("Connection failed.");
            return connSt;
        case STATUS_TIMEOUT:
            st->error("Connection timed out.");
            return connSt;
        default:
            break;
    }

    if (!setBlocking(true)) {
        st->error("An unexpected error occurred: " + string(strerror(errno)));
        return STATUS_BAD_CONNECT;
    }

    st->set("Connected; authenticate with /auth <user> <pass>");
    term.getInputWindow()->setTag(host);

    return STATUS_OK;
}

bool ServerConn::openSocket() {
    if ((socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) return false;
    return true;
}

bool ServerConn::verifyHost(sockaddr_in &addr) {
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
        // try name lookup
        string ip;
        tuna::host2ip(host, ip);
        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
            return false;
        }
    }
    return true;
}

bool ServerConn::setBlocking(bool blocking) {
    int arg;
    if ((arg = fcntl(socket, F_GETFL, nullptr)) < 0) return false;
    if (!blocking) {
        arg |= O_NONBLOCK;
    } else {
        arg &= (~O_NONBLOCK);
    }
    if (fcntl(socket, F_SETFL, arg) < 0) return false;
    return true;
}

int ServerConn::connect(sockaddr_in &addr) {
    fd_set fds;
    bool failed = false;

    timeval tv{};
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    if (::connect(socket, (sockaddr*) &addr, sizeof(addr)) < 0) {
        if (errno == EINPROGRESS) {
            FD_ZERO(&fds);
            FD_SET(socket, &fds);
            if (select(socket + 1, nullptr, &fds, nullptr, &tv) == 1) {
                int err;
                socklen_t len = sizeof(err);
                getsockopt(socket, SOL_SOCKET, SO_ERROR, &err, &len);
                failed = err != 0;
            } else {
                return STATUS_TIMEOUT;
            }
        } else {
            failed = true;
        }
    }
    if (failed) return STATUS_BAD_CONNECT;
    return STATUS_OK;
}

///
/// == Other private methods ==
///

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
