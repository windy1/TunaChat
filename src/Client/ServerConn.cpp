//
// Created by Walker Crouse on 9/26/18.
//

#include "ServerConn.h"
#include "Terminal/windows.h"
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
    sprintf(authStr, "%s:%s:%s", PROTO_AUTH, user.c_str(), pwd.c_str());
    string res;
    getResponse(string(authStr), res);

    StatusPtr st = client.getTerminal().getStatusWindow();
    if (strcmp(res.c_str(), PROTO_AUTHYES) == 0) {
        authenticated = true;
        st->clear();
        st->addStr(0, 0, "Successfully authenticated as " + user);
        client.getTerminal().getInputWindow()->setTag(user);

        client.getTerminal().getMainWindow()->log("DEBUG");

        msgChan = make_shared<MessageChannel>(*this);

        return STATUS_OK;
    } else if (strcmp(res.c_str(), PROTO_AUTHNO) == 0) {
        st->error("Invalid username or password");
        return STATUS_INVALID_ARG;
    } else {
        st->error("An unknown server error occurred");
        return STATUS_BAD_RESPONSE;
    }
}

int ServerConn::sendMessage(const string &user, const string &text) {
    char req[1024];
    sprintf(req, "%s:%s:%s", PROTO_TO, user.c_str(), text.c_str());
    send(socket, req, strlen(req), 0);
    return STATUS_OK;
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
        statusWin->error("Invalid host name.");
        return STATUS_INVALID_ARG;
    }

    if (::connect(socket, (sockaddr*) &addr, sizeof(addr)) < 0) {
        statusWin->error("Connection failed.");
        return STATUS_BAD_CONNECT;
    }

    statusWin->clear();
    statusWin->addStr(0, 0, "Connected; authenticate with /auth <user> <pass>");

    term.getInputWindow()->setTag(host);

    return STATUS_OK;
}

int ServerConn::sayHello() {
    string res;
    getResponse(PROTO_HELLO, res);
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
