//
// Created by Walker Crouse on 9/24/18.
//

#include "ClientConnection.h"

ClientConnection::ClientConnection(ChatServer &server, int socket) : server(server), socket(socket) {}

const ChatServer& ClientConnection::getServer() const {
    return server;
}

int ClientConnection::getSocket() const {
    return socket;
}

const std::string* ClientConnection::getLastMessage() const {
    return &lastMessage;
}

const User* ClientConnection::getUser() const {
    return user;
}

int ClientConnection::getError() const {
    return error;
}

const std::string* ClientConnection::readLine() {
    int bufferSize = server.getBufferSize();
    char buffer[bufferSize];
    ssize_t readSize = recv(socket, buffer, (size_t) bufferSize, 0);
    if (readSize < 0) return (std::string*) close("No data received", ERR_NO_DATA);
    std::stringstream in(buffer);
    in >> lastMessage;
    return &lastMessage;
}

bool ClientConnection::verify() {
    if (strcmp(readLine()->c_str(), PROTO_HELLO) == 0) {
        write(socket, PROTO_HELLO, strlen(PROTO_HELLO));
        write(socket, "\n", 1);
        return true;
    }
    close("Invalid handshake.", ERR_BAD_HS);
    return false;
}

bool ClientConnection::authenticate() {
    const std::string* authLine = readLine();
    unsigned long sep1 = authLine->find(':');
    unsigned long sep2 = authLine->rfind(':');
    if (sep1 == std::string::npos || sep2 == std::string::npos) {
        close("Invalid authentication line.", ERR_AUTH_FORMAT);
        return false;
    }

    std::string header = authLine->substr(0, sep1);
    std::string user = authLine->substr(sep1 + 1, sep2 - header.size() - 1);
    std::string pwd = authLine->substr(sep2 + 1, std::string::npos);

    printf("header = %s\n", header.c_str());
    printf("user = %s\n", user.c_str());
    printf("pwd = %s\n", pwd.c_str());

    if (strcmp(header.c_str(), PROTO_HEAD_AUTH) != 0) {
        close("Invalid authentication line.", ERR_AUTH_FORMAT);
        return false;
    }

    this->user = server.authenticate(*this, user, pwd);
    if (this->user != nullptr) {
        write(socket, PROTO_AUTHYES, strlen(PROTO_AUTHYES));
        write(socket, "\n", 1);
        write(socket, PROTO_HEAD_SIGNIN, strlen(PROTO_HEAD_SIGNIN));
        write(socket, ":", 1);
        write(socket, user.c_str(), user.size());
        write(socket, "\n", 1);
        return true;
    }

    write(socket, PROTO_AUTHNO, strlen(PROTO_AUTHNO));
    write(socket, "\n", 1);

    return false;
}

void ClientConnection::processCommand() {
    const std::string* cmd = readLine();
    if (strcmp(cmd->c_str(), PROTO_CMD_LIST) == 0) {
        writeUserList();
    } else if (strcmp(cmd->c_str(), PROTO_CMD_BYE) == 0) {
        server.signOff(user->getName());
        write(socket, PROTO_SIGNOFF, strlen(PROTO_SIGNOFF));
        write(socket, ":", 1);
        write(socket, user->getName().c_str(), user->getName().size());
        write(socket, "\n", 1);
        close("Connection terminated.", ERR_NONE_TERM);
    } else {
        unsigned long sep1 = cmd->find(':');
        unsigned long sep2 = cmd->rfind(':');
        if (sep1 == std::string::npos || sep2 == std::string::npos) {
            close("Invalid command.", ERR_CMD_FORMAT);
            return;
        }

        std::string header = cmd->substr(0, sep1);
        std::string user = cmd->substr(sep1 + 1, sep2 - header.size() - 1);
        std::string text = cmd->substr(sep2 + 1, std::string::npos);

        printf("header = %s\n", header.c_str());
        printf("user = %s\n", user.c_str());
        printf("text = %s\n", text.c_str());

        if (strcmp(header.c_str(), PROTO_CMD_TO) != 0) {
            close("Invalid command.", ERR_CMD_FORMAT);
            return;
        }

        server.dispatch(user, this->user->getName(), text);
    }
}

void ClientConnection::sendMessage(const std::string &from, const std::string &text) {
    write(socket, PROTO_CMD_FROM, strlen(PROTO_CMD_FROM));
    write(socket, ":", 1);
    write(socket, from.c_str(), from.size());
    write(socket, ":", 1);
    write(socket, text.c_str(), text.size());
    write(socket, "\n", 1);
}

void ClientConnection::writeUserList() {
    const std::vector<User> &userList = server.getUserList();
    for (int i = 0; i < userList.size(); i++) {
        const std::string &name = userList[i].getName();
        write(socket, name.c_str(), name.size());
        if (i < userList.size() - 1) write(socket, ",", 1);
    }
    write(socket, "\n", 1);
}

void* ClientConnection::close(std::string msg, int err) {
    fprintf(stderr, "Error: %s (%d)\n", msg.c_str(), err);
    ::close(socket);
    error = err;
    return nullptr;
}
