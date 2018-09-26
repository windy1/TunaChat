//
// Created by Walker Crouse on 9/24/18.
//

#include "ClientConn.h"
#include "User.h"
#include <arpa/inet.h>

using std::stringstream;

/**
 * Thread function for handling new incoming connections. Encapsulates the entire lifetime of a ClientConn and enforces
 * protocol.
 *
 * @param cli client connection
 * @return status code
 */
int handleConnection(ClientConn *cli);

///
/// == Statics ==
///

const string ClientConn::NAME_UNKNOWN = "UNKNOWN";
int ClientConn::LAST_ID = 0;

///
/// == ClientConn ==
///

ClientConn::ClientConn(ChatServer &server, sockaddr_in address, int socket) :
    server(server),
    address(address),
    socket(socket),
    id(LAST_ID++),
    th(thread(handleConnection, this)) {}

///
/// == Methods ==
///

bool ClientConn::verify() {
    if (strcmp(readLine()->c_str(), PROTO_HELLO) == 0) {
        write(socket, PROTO_HELLO, strlen(PROTO_HELLO));
        write(socket, "\n", 1);
        return true;
    }
    close("INVALID_HANDSHAKE", STATUS_BAD_REQUEST);
    return false;
}

bool ClientConn::authenticate() {
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

void ClientConn::processCommand() {
    const string* cmd = readLine();

    if (status == STATUS_SHUTDOWN) {
        return;
    }

    if (strcmp(cmd->c_str(), PROTO_LIST) == 0) {
        writeUserList();
    } else if (strcmp(cmd->c_str(), PROTO_BYE) == 0) {
        server.signOff(user->getName());
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

void ClientConn::sendMessage(const string &from, const string &text) {
    printf("SEND [%s => %s]\n", from.c_str(), getTag().c_str());
    write(socket, PROTO_FROM, strlen(PROTO_FROM));
    write(socket, ":", 1);
    write(socket, from.c_str(), from.size());
    write(socket, ":", 1);
    write(socket, text.c_str(), text.size());
    write(socket, "\n", 1);
}

void ClientConn::shutdown() {
    printf("SHUTDOWN [%s]\n", getTag().c_str());
    write(socket, PROTO_SIGNOFF, strlen(PROTO_SIGNOFF));
    write(socket, ":", 1);
    write(socket, user->getName().c_str(), user->getName().size());
    write(socket, "\n", 1);
    status = STATUS_SHUTDOWN;
    // shutdown() disables further operations on the socket while still keeping the socket descriptor so that if recv()
    // is blocking, the connection can be closed gracefully from another thread
    ::shutdown(socket, SHUT_RDWR);
}

void* ClientConn::close(const string &msg, int status) {
    fprintf(stderr, "%s [%s] (code: %d)\n", msg.c_str(), getTag().c_str(), status);
    ::close(socket);
    this->status = status;
    return nullptr;
}

///
/// == Getters ==
///

ChatServer& ClientConn::getServer() const {
    return server;
}

UserPtr ClientConn::getUser() const {
    return user;
}

const string& ClientConn::getTag() const {
    string name = NAME_UNKNOWN;
    string addr = inet_ntoa(address.sin_addr);
    return tag = name + '@' + addr;
}

int ClientConn::getId() const {
    return id;
}

const sockaddr_in& ClientConn::getAddress() const {
    return address;
}

int ClientConn::getSocket() const {
    return socket;
}

const string* ClientConn::getLastMessage() const {
    return &lastMessage;
}

int ClientConn::getStatus() const {
    return status;
}

thread& ClientConn::getThread() {
    return th;
}

///
/// == Private methods ==
///

const string* ClientConn::readLine() {
    int bufferSize = server.getBufferSize();
    char buffer[bufferSize];
    ssize_t readSize = recv(socket, buffer, (size_t) bufferSize, 0);

    if (status == STATUS_SHUTDOWN) {
        // check for shutdown signal that may have been set async while waiting for recv
        return nullptr;
    }

    if (readSize < 0) {
        return (string*) close("NO_DATA", STATUS_NO_DATA);
    }

    stringstream in(buffer);
    in >> lastMessage;
    return &lastMessage;
}

void ClientConn::writeUserList() {
    printf("LIST [%s]\n", getTag().c_str());
    const vector<UserPtr> &userList = server.getUserList();
    for (int i = 0; i < userList.size(); i++) {
        const string &name = userList[i]->getName();
        write(socket, name.c_str(), name.size());
        if (i < userList.size() - 1) write(socket, ",", 1);
    }
    write(socket, "\n", 1);
}

///
/// == Main method for new connections ==
///

int handleConnection(ClientConn *cli) {
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

    if (cli->getStatus() == STATUS_SHUTDOWN) {
        cli->close("GOODBYE", STATUS_CLOSED);
    }

    return cli->getStatus();
}