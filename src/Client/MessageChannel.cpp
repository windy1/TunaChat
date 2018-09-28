//
// Created by Walker Crouse on 9/26/18.
//

#include "MessageChannel.h"
#include "Terminal/windows.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

using std::stringstream;

MessageChannel::MessageChannel(ServerConn &conn) : conn(conn), th(thread(&MessageChannel::start, this)) {}

int MessageChannel::start() {
    Terminal &term = conn.getClient().getTerminal();
    MainPtr main = term.getMainWindow();
    StatusPtr st = term.getStatusWindow();

    string header;
    string body1;
    string body2;

    while (status != STATUS_CLOSED) {
        string data;
        readLine(data, conn.getSocket(), conn.getBufferSize());

        if (!parse3(data, header, body1, body2)) {
            parse2(data, header, body1);
        }

        if (header == PROTO_FROM) {
            main->log(body1, body2);
        } else if (header == PROTO_SIGNOFF) {
            char str[100];
            sprintf(str, "%s has signed off", body1.c_str());
            main->log(str);
        } else {
//            stringstream in(data);
//            string user;
//            main->log("Online users:");
//            while (getline(in, user, ',')) {
//                main->log("  * " + user);
//            }
        }
    }
    return status;
}

ServerConn& MessageChannel::getConnection() const {
    return conn;
}

int MessageChannel::getStatus() const {
    return status;
}
