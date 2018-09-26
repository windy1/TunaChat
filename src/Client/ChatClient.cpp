//
// Created by Walker Crouse on 9/25/18.
//

#include "ChatClient.h"
#include "Terminal/InputWindow.h"
#include "Terminal/StatusWindow.h"
#include "Terminal/MainWindow.h"
#include "ServerConn.h"
#include "Command.h"
#include <sstream>

using std::stringstream;
using std::make_shared;

ChatClient::ChatClient() {
    commands.push_back(make_shared<Command>(
        this, "quit", &ChatClient::quit, "Usage: /quit"));
    commands.push_back(make_shared<Command>(
        this, "connect", &ChatClient::connect, "Usage: /connect <host> [port]", 2, 1));
    commands.push_back(make_shared<Command>(
        this, "auth", &ChatClient::authenticate, "Usage: /auth <user> <pass>", 2, 2));
    commands.push_back(make_shared<Command>(
        this, "tell", &ChatClient::tell, "Usage: /tell <user> <message>", -1, 2));
}

int ChatClient::start() {
    MainPtr main = term.getMainWindow();
    StatusPtr statusWin = term.getStatusWindow();
    InputPtr input = term.getInputWindow();

    main->draw();
    main->refresh();

    while (status != STATUS_CLOSED) {

        input->clear();
        input->draw();

        statusWin->refresh();
        input->refresh();

        string strIn;
        input->getStr(strIn);
        status = processInput(strIn);
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
            statusWin->drawError("Invalid port number.");
            return STATUS_INVALID_ARG;
        }
    }

    char msg[1024];
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
        term.getStatusWindow()->drawError("You are not connected to a server");
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
        term.getStatusWindow()->drawError("You are not connected to a server");
        return STATUS_OK;
    }
}

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
