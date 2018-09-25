//
// Created by Walker Crouse on 9/24/18.
//

#include "CliConn.h"
#include "User.h"
#include <arpa/inet.h>

using std::stringstream;

const string CliConn::NAME_UNKNOWN = "UNKNOWN";
int CliConn::LAST_ID = 0;

CliConn::CliConn(ChatServer &server, sockaddr_in address, int socket) :
    server(server),
    address(address),
    socket(socket),
    id(LAST_ID++) {}

bool CliConn::verify() {
    if (strcmp(readLine()->c_str(), PROTO_HELLO) == 0) {
        write(socket, PROTO_HELLO, strlen(PROTO_HELLO));
        write(socket, "\n", 1);
        return true;
    }
    close("INVALID_HANDSHAKE", STATUS_BAD_REQUEST);
    return false;
}

bool CliConn::authenticate() {
    const string* authLine = readLine();
    unsigned long sep1 = authLine->find(':');
    unsigned long sep2 = authLine->rfind(':');
    if (sep1 == string::npos || sep2 == string::npos || sep1 == sep2) {
        close("AUTH_LINE_FORMAT", STATUS_BAD_REQUEST);
        return false;
    }

    string header = authLine->substr(0, sep1);
    string user = authLine->substr(sep1 + 1, sep2 - header.size() - 1);
    string pwd = authLine->substr(sep2 + 1, string::npos);

    if (strcmp(header.c_str(), PROTO_AUTH) != 0) {
        close("AUTH_LINE_FORMAT", STATUS_BAD_REQUEST);
        return false;
    }

    this->user = server.authenticate(user, pwd);
    if (this->user != nullptr) {
        write(socket, PROTO_AUTHYES, strlen(PROTO_AUTHYES));
        write(socket, "\n", 1);
        write(socket, PROTO_SIGNIN, strlen(PROTO_SIGNIN));
        write(socket, ":", 1);
        write(socket, user.c_str(), user.size());
        write(socket, "\n", 1);
        return true;
    }

    write(socket, PROTO_AUTHNO, strlen(PROTO_AUTHNO));
    write(socket, "\n", 1);

    return false;
}

void CliConn::processCommand() {
    const string* cmd = readLine();

    if (status == STATUS_SHUTDOWN) {
        close("GOODBYE", STATUS_CLOSED);
        return;
    }

    if (strcmp(cmd->c_str(), PROTO_LIST) == 0) {
        writeUserList();
    } else if (strcmp(cmd->c_str(), PROTO_BYE) == 0) {
        server.signOff(user->getName());
        close("INITIATED_SIGNOFF", STATUS_CLOSED);
    } else {
        unsigned long sep1 = cmd->find(':');
        unsigned long sep2 = cmd->rfind(':');
        if (sep1 == string::npos || sep2 == string::npos) {
            close("INVALID_COMMAND", STATUS_BAD_REQUEST);
            return;
        }

        string header = cmd->substr(0, sep1);
        string usr = cmd->substr(sep1 + 1, sep2 - header.size() - 1);
        string text = cmd->substr(sep2 + 1, string::npos);

        if (strcmp(header.c_str(), PROTO_TO) != 0) {
            close("INVALID_COMMAND", STATUS_BAD_REQUEST);
            return;
        }

        server.dispatch(usr, user->getName(), text);
    }
}

void CliConn::sendMessage(const string &from, const string &text) {
    printf("SEND [%s => %s]\n", from.c_str(), getTag().c_str());
    write(socket, PROTO_FROM, strlen(PROTO_FROM));
    write(socket, ":", 1);
    write(socket, from.c_str(), from.size());
    write(socket, ":", 1);
    write(socket, text.c_str(), text.size());
    write(socket, "\n", 1);
}

void CliConn::kick() {
    printf("KICK [%s]\n", getTag().c_str());
    write(socket, PROTO_SIGNOFF, strlen(PROTO_SIGNOFF));
    write(socket, ":", 1);
    write(socket, user->getName().c_str(), user->getName().size());
    write(socket, "\n", 1);
    status = STATUS_SHUTDOWN;
    // shutdown() disables further operations on the socket while still keeping the socket descriptor so that if recv()
    // is blocking, the connection can be closed gracefully from another thread
    shutdown(socket, SHUT_RDWR);
}

ChatServer& CliConn::getServer() const {
    return server;
}

UserPtr CliConn::getUser() const {
    return user;
}

const string& CliConn::getUsername() const {
    if (user == nullptr) {
        return NAME_UNKNOWN;
    } else if (username.empty()) {
        return username = user->getName();
    } else {
        return username;
    }
}

const sockaddr_in& CliConn::getAddress() const {
    return address;
}

const string& CliConn::getAddressString() const {
    if (addrStr.empty()) {
        addrStr = inet_ntoa(address.sin_addr);
    }
    return addrStr;
}

const string& CliConn::getTag() const {
    return tag = getUsername() + '@' + getAddressString();
}

int CliConn::getSocket() const {
    return socket;
}

const string* CliConn::getLastMessage() const {
    return &lastMessage;
}

int CliConn::getStatus() const {
    return status;
}

const string* CliConn::readLine() {
    int bufferSize = server.getBufferSize();
    char buffer[bufferSize];
    ssize_t readSize = recv(socket, buffer, (size_t) bufferSize, 0);

    if (status == STATUS_SHUTDOWN) {
        return nullptr;
    }

    if (readSize < 0) {
        return (string*) close("NO_DATA", STATUS_NO_DATA);
    }

    stringstream in(buffer);
    in >> lastMessage;
    return &lastMessage;
}

void CliConn::writeUserList() {
    printf("LIST [%s]\n", getTag().c_str());
    const vector<UserPtr> &userList = server.getUserList();
    for (int i = 0; i < userList.size(); i++) {
        const string &name = userList[i]->getName();
        write(socket, name.c_str(), name.size());
        if (i < userList.size() - 1) write(socket, ",", 1);
    }
    write(socket, "\n", 1);
}

void* CliConn::close(string msg, int status) {
    fprintf(stderr, "%s [%s] (code: %d)\n", msg.c_str(), getTag().c_str(), status);
    ::close(socket);
    this->status = status;
    server.onConnectionClosed(this);
    return nullptr;
}
