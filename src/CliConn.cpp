//
// Created by Walker Crouse on 9/24/18.
//

#include "CliConn.h"

using std::stringstream;

CliConn::CliConn(ChatServer &server, int socket) : server(server), socket(socket) {}

bool CliConn::verify() {
    if (strcmp(readLine()->c_str(), PROTO_HELLO) == 0) {
        write(socket, PROTO_HELLO, strlen(PROTO_HELLO));
        write(socket, "\n", 1);
        return true;
    }
    close("Invalid handshake.", ERR_BAD_HELLO);
    return false;
}

bool CliConn::authenticate() {
    const string* authLine = readLine();
    unsigned long sep1 = authLine->find(':');
    unsigned long sep2 = authLine->rfind(':');
    if (sep1 == string::npos || sep2 == string::npos) {
        close("Invalid authentication line.", ERR_AUTH_FORMAT);
        return false;
    }

    string header = authLine->substr(0, sep1);
    string user = authLine->substr(sep1 + 1, sep2 - header.size() - 1);
    string pwd = authLine->substr(sep2 + 1, string::npos);

    if (strcmp(header.c_str(), PROTO_AUTH) != 0) {
        close("Invalid authentication line.", ERR_AUTH_FORMAT);
        return false;
    }

    this->user = server.authenticate(*this, user, pwd);
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
    if (strcmp(cmd->c_str(), PROTO_LIST) == 0) {
        writeUserList();
    } else if (strcmp(cmd->c_str(), PROTO_BYE) == 0) {
        server.signOff(user->getName());
    } else {
        unsigned long sep1 = cmd->find(':');
        unsigned long sep2 = cmd->rfind(':');
        if (sep1 == string::npos || sep2 == string::npos) {
            close("Invalid command.", ERR_CMD_FORMAT);
            return;
        }

        string header = cmd->substr(0, sep1);
        string usr = cmd->substr(sep1 + 1, sep2 - header.size() - 1);
        string text = cmd->substr(sep2 + 1, string::npos);

        if (strcmp(header.c_str(), PROTO_TO) != 0) {
            close("Invalid command.", ERR_CMD_FORMAT);
            return;
        }

        server.dispatch(usr, user->getName(), text);
    }
}

void CliConn::sendMessage(const string &from, const string &text) {
    write(socket, PROTO_FROM, strlen(PROTO_FROM));
    write(socket, ":", 1);
    write(socket, from.c_str(), from.size());
    write(socket, ":", 1);
    write(socket, text.c_str(), text.size());
    write(socket, "\n", 1);
}

void CliConn::kick() {
    write(socket, PROTO_SIGNOFF, strlen(PROTO_SIGNOFF));
    write(socket, ":", 1);
    write(socket, user->getName().c_str(), user->getName().size());
    write(socket, "\n", 1);
    error = ERR_NONE_TERM;
    ::close(socket);
}

ChatServer& CliConn::getServer() const {
    return server;
}

UserPtr CliConn::getUser() const {
    return user;
}

int CliConn::getSocket() const {
    return socket;
}

const string* CliConn::getLastMessage() const {
    return &lastMessage;
}

int CliConn::getError() const {
    return error;
}

const string* CliConn::readLine() {
    int bufferSize = server.getBufferSize();
    char buffer[bufferSize];
    ssize_t readSize = recv(socket, buffer, (size_t) bufferSize, 0);
    if (readSize < 0) return (string*) close("No data received", ERR_NO_DATA);
    stringstream in(buffer);
    in >> lastMessage;
    return &lastMessage;
}

void CliConn::writeUserList() {
    const vector<UserPtr> &userList = server.getUserList();
    for (int i = 0; i < userList.size(); i++) {
        const string &name = userList[i]->getName();
        write(socket, name.c_str(), name.size());
        if (i < userList.size() - 1) write(socket, ",", 1);
    }
    write(socket, "\n", 1);
}

void* CliConn::close(string msg, int err) {
    fprintf(stderr, "Error: %s (%d)\n", msg.c_str(), err);
    ::close(socket);
    error = err;
    return nullptr;
}
