//
// Created by Walker Crouse on 9/25/18.
//

#include "ChatClient.h"
#include "Terminal/windows.h"
#include "ServerConn.h"
#include "Command.h"
#include <sstream>

using std::stringstream;
using std::make_shared;

///
/// == ChatClient ==
///

ChatClient::ChatClient() {
    commands = {
        make_shared<Command>(this, "quit", &ChatClient::quit, "Usage: /quit"),
        make_shared<Command>(this, "connect", &ChatClient::connect, "Usage: /connect <host> [port]", 2, 1),
        make_shared<Command>(this, "auth", &ChatClient::authenticate, "Usage: /auth <user> <pass>", 2, 2),
        make_shared<Command>(this, "tell", &ChatClient::tell, "Usage: /tell <user> <message>", -1, 2),
        make_shared<Command>(this, "list", &ChatClient::list, "Usage: /list")
    };
}

///
/// == Methods ==
///

int ChatClient::start() {
    MainPtr main = term.getMainWindow();
    StatusPtr statusWin = term.getStatusWindow();
    InputPtr input = term.getInputWindow();

    main->refresh();

    while (status != STATUS_CLOSED) {
        input->clear();
        input->divider();
        input->tag();

        statusWin->divider();

        main->flush();

        main->refresh();
        statusWin->refresh();
        input->refresh();

        string strIn;
        input->getStr(strIn);

        status = processInput(strIn);

        //main->log("walker", strIn);
    }

    return status;
}

int ChatClient::processInput(const string &in) {
    vector<string> args;
    Command::parseArgs(in, args);
    for (auto &cmd : commands) {
        if (cmd->matches(in)) {
            return cmd->execute(args);
        }
    }
    return STATUS_UNKNOWN_CMD;
}

///
/// == Command executors ==
///

int ChatClient::quit(const vector<string> &args) {
    return STATUS_CLOSED;
}

int ChatClient::connect(const vector<string> &args) {
    StatusPtr statusWin = term.getStatusWindow();

    string host = args[0];
    if (host == "localhost") {
        host = "127.0.0.1";
    }

    int port = 12000;
    if (args.size() > 1) {
        try {
            port = stoi(args[1]);
        } catch (...) {
            statusWin->error("Invalid port number.");
            return STATUS_INVALID_ARG;
        }
    }

    char msg[100];
    sprintf(msg, "Connecting to %s %d...", host.c_str(), port);
    statusWin->addStr(0, 0, msg);

    conn = make_shared<ServerConn>(*this, host, port);

    return STATUS_OK;
}

int ChatClient::authenticate(const vector<string> &args) {
    string user = args[0];
    string pwd = args[1];
    if (conn != nullptr) {
        conn->authenticate(user, pwd);
        return STATUS_OK;
    } else {
        term.getStatusWindow()->error("You are not connected to a server");
        return STATUS_OK;
    }
}

int ChatClient::tell(const vector<string> &args) {
    string user = args[0];
    string msg;
    for (int i = 1; i < args.size(); i++) {
        msg += args[i];
        if (i < args.size() - 1) msg += ' ';
    }
    if (conn != nullptr) {
        conn->sendMessage(user, msg);
        return STATUS_OK;
    } else {
        term.getStatusWindow()->error("You are not connected to a server");
        return STATUS_OK;
    }
}

int ChatClient::list(const vector<string> &args) {
    if (conn != nullptr) {
        conn->requestList();
    } else {
        term.getStatusWindow()->error("You are not connected to a server");
    }
    return STATUS_OK;
}

///
/// == Getters ==
///

Terminal& ChatClient::getTerminal() const {
    return (Terminal&) term;
}

const vector<CmdPtr>& ChatClient::getCommands() const {
    return commands;
}

ServerConnPtr ChatClient::getConnection() const {
    return conn;
}

int ChatClient::getStatus() const {
    return status;
}
