//
// Created by Walker Crouse on 9/26/18.
//

#include "MessageChannel.h"
#include "Terminal/windows.h"
#include "Terminal/MainWindow.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

using std::stringstream;

///
/// == MessageChannel ==
///

MessageChannel::MessageChannel(ServerConn &conn) : conn(conn), th(thread(&MessageChannel::start, this)) {}

///
/// == Methods ==
///

int MessageChannel::start() {
    ChatClient &client = conn.getClient();
    Terminal &term = client.getTerminal();
    MainPtr main = term.getMainWindow();
    StatusPtr st = term.getStatusWindow();
    InputPtr input = term.getInputWindow();
    UserListPtr userList = term.getUserListWindow();

    string header;
    string body1;
    string body2;

    main->log("You have joined the server.");

    while (status != STATUS_CLOSED) {
        if (client.isWaiting()) {
            main->flush(*st);
            main->refresh();
            if (userList->isOpened()) {
                if (time(nullptr) - userList->getOpenTime() >= userList->getRefreshRate()) {
                    // refresh the open user list
                    conn.requestList();
                    userList->resetTime();
                }
                userList->refresh();
            }
            input->refresh();
        }

        string data;
        tuna::readLine(data, conn.getSocket(), conn.getBufferSize());

        if (status == STATUS_SHUTDOWN) return status;

        //main->debug("data = " + data);

        if (!tuna::parse3(data, header, body1, body2) && !tuna::parse2(data, header, body1)) {
            stringstream in(data);
            string user;
            vector<string> users;
            while (getline(in, user, ',')) {
                if (user[0] == ' ') user = user.substr(1);
                users.push_back(user);
            }
            userList->set(users);
            continue;
        }

        if (header == PROTO_FROM) {
            main->log(body1, body2);
        } else if (header == PROTO_SIGNOFF) {
            char str[100];
            sprintf(str, "%s has signed off", body1.c_str());
            main->log(str);
        } else if (header == PROTO_SIGNIN) {
            char str[100];
            sprintf(str, "%s has signed in", body1.c_str());
            main->log(str);
        }
    }
    return status;
}

int MessageChannel::shutdown() {
    return status = STATUS_SHUTDOWN;
}

///
/// == Getters ==
///

ServerConn& MessageChannel::getConnection() const {
    return conn;
}

thread& MessageChannel::getThread() const {
    return (thread&) th;
}

int MessageChannel::getStatus() const {
    return status;
}
