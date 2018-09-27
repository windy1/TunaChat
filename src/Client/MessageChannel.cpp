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
    while (status != STATUS_CLOSED) {
        Terminal &term = conn.getClient().getTerminal();
        term.getMainWindow()->log("DEGUG2");

        string data;
        readLine(data, conn.getSocket(), conn.getBufferSize());

        term.getMainWindow()->log("DEGUG3");

        string header;
        string body1;
        string body2;

        StatusPtr st = term.getStatusWindow();

        if (!parse3(data, header, body1, body2) && !parse2(data, header, body1)) {
            char err[100];
            sprintf(err, "Received bad data from server [%s]", data.c_str());
            st->error(err);
            continue;
        }

        if (header == PROTO_FROM) {
            term.getMainWindow()->log("message got");
            term.getMainWindow()->log(body1, body2);
        } else if (header == PROTO_SIGNOFF) {

        } else {
            char err[100];
            sprintf(err, "Received unknown header from server [%s]", header.c_str());
            st->error(err);
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
