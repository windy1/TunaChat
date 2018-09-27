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
        char data[1024];
        sprintf(data, "%s\n", PROTO_HELLO);
        write(socket, data, strlen(data));
        return true;
    }
    close("INVALID_HANDSHAKE", STATUS_BAD_REQUEST);
    return false;
}

bool ClientConn::authenticate() {
    const string* authLine = readLine();
    string header;
    string user;
    string pwd;
    if (!parse3(*authLine, header, user, pwd)) {
        close("AUTH_LINE_FORMAT", STATUS_BAD_REQUEST);
        return false;
    }

    if (strcmp(header.c_str(), PROTO_AUTH) != 0) {
        close("AUTH_LINE_FORMAT", STATUS_BAD_REQUEST);
        return false;
    }

    this->user = server.authenticate(user, pwd);
    if (this->user != nullptr) {
        char data[1024];
        sprintf(data, "%s\n%s:%s\n", PROTO_AUTHYES, PROTO_SIGNIN, user.c_str());
        write(socket, data, strlen(data));
        return true;
    }

    char data[1024];
    sprintf(data, "%s\n", PROTO_AUTHNO);
    write(socket, data, strlen(data));

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
        string header;
        string usr;
        string text;
        if (!parse3(*cmd, header, usr, text)) {
            close("INVALID_COMMAND", STATUS_BAD_REQUEST);
            return;
        }

//        unsigned long sep1 = cmd->find(':');
//        unsigned long sep2 = cmd->rfind(':');
//        if (sep1 == string::npos || sep2 == string::npos) {
//            close("INVALID_COMMAND", STATUS_BAD_REQUEST);
//            return;
//        }

        if (strcmp(header.c_str(), PROTO_TO) != 0) {
            close("INVALID_COMMAND", STATUS_BAD_REQUEST);
            return;
        }

        server.dispatch(usr, user->getName(), text);
    }
}

void ClientConn::sendMessage(const string &from, const string &text) {
    printf("SEND [%s => %s]\n", from.c_str(), getTag().c_str());
    char data[1024];
    sprintf(data, "%s:%s:%s\n", PROTO_FROM, from.c_str(), text.c_str());
    write(socket, data, strlen(data));
}

void ClientConn::shutdown() {
    printf("SHUTDOWN [%s]\n", getTag().c_str());
    status = STATUS_SHUTDOWN;
    // shutdown() disables further operations on the socket while still keeping the socket descriptor so that if recv()
    // is blocking, the connection can be closed gracefully from another thread
    ::shutdown(socket, SHUT_RDWR);
}

void ClientConn::goodbye(UserPtr user) {
    char data[1024];
    sprintf(data, "%s:%s\n", PROTO_SIGNOFF, user->getName().c_str());
    write(socket, data, strlen(data));
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
    if (user != nullptr) name = user->getName();
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
    string data;
    int readSize = ::readLine(data, socket, server.getBufferSize());

    if (status == STATUS_SHUTDOWN) {
        // check for shutdown signal that may have been set async while waiting for recv
        return nullptr;
    }

    if (readSize < 0) {
        return (string*) close("NO_DATA", STATUS_NO_DATA);
    }

    return &(lastMessage = data);
}

void ClientConn::writeUserList() {
    printf("LIST [%s]\n", getTag().c_str());
    const vector<UserPtr> &userList = server.getUserList();
    string data;
    for (int i = 0; i < userList.size(); i++) {
        const string &name = userList[i]->getName();
        data += name;
        if (i < userList.size() - 1) data += ',';
    }
    data += '\n';
    write(socket, data.c_str(), data.size());
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
