//
// Created by Walker Crouse on 9/25/18.
//

#include "ChatClient.h"
#include "Terminal/windows.h"
#include "Terminal/MainWindow.h"
#include "ServerConn.h"
#include "Command.h"
#include <sstream>
#include <fstream>

using std::stringstream;
using std::make_shared;
using std::ifstream;

///
/// == Statics ==
///

const string ChatClient::TITLE_FILE = "files/title.txt";
const string ChatClient::HELP_FILE = "files/help.txt";

///
/// == ChatClient ==
///

ChatClient::ChatClient() {
    commands = {
        make_shared<Command>(this, "quit", &ChatClient::quit, "Usage: /quit"),
        make_shared<Command>(this, "connect", &ChatClient::connect, "Usage: /connect <host> [port]", 2, 1),
        make_shared<Command>(this, "auth", &ChatClient::authenticate, "Usage: /auth <user> <pass>", 2, 2, true),
        make_shared<Command>(this, "tell", &ChatClient::tell, "Usage: /tell <user> <message>", -1, 2, true, true),
        make_shared<Command>(this, "list", &ChatClient::list, "Usage: /list", -1, -1, true, true),
        make_shared<Command>(this, "disconnect", &ChatClient::disconnect, "Usage: /disconnect", -1, -1, true),
        make_shared<Command>(this, "help", &ChatClient::help, "Usage: /help")
    };
}

///
/// == Methods ==
///

int ChatClient::start() {
    MainPtr main = term.getMainWindow();
    StatusPtr st = term.getStatusWindow();
    InputPtr input = term.getInputWindow();
    CenterPtr center = term.getCenterWindow();

    main->refresh();

    showWelcome();

    while (status != STATUS_CLOSED) {
        st->divider();
        st->refresh();

        main->flush();
        main->refresh();

        input->reset();
        input->refresh();

        waiting = true;
        string strIn;
        input->getStr(strIn);
        waiting = false;

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
    term.getStatusWindow()->error("Unknown command.");
    return STATUS_UNKNOWN_CMD;
}

///
/// == Command executors ==
///

int ChatClient::quit(const vector<string> &args) {
    if (conn != nullptr) {
        conn->disconnect();
        conn = nullptr;
    }
    return STATUS_CLOSED;
}

int ChatClient::connect(const vector<string> &args) {
    StatusPtr st = term.getStatusWindow();
    string host = args[0];
    int port = DEFAULT_PORT;

    if (args.size() > 1) {
        try {
            port = stoi(args[1]);
        } catch (...) {
            st->error("Invalid port number.");
            return STATUS_INVALID_ARG;
        }
    }

    char msg[100];
    sprintf(msg, "Connecting to %s %d...", host.c_str(), port);
    st->set(msg);
    st->divider();
    st->refresh();

    if ((conn = make_shared<ServerConn>(*this, host, port))->getStatus() != STATUS_OK) {
        conn = nullptr;
    }

    return STATUS_OK;
}

int ChatClient::authenticate(const vector<string> &args) {
    string user = args[0];
    string pwd = args[1];
    conn->authenticate(user, pwd);
    return STATUS_OK;
}

int ChatClient::tell(const vector<string> &args) {
    string user = args[0];
    string msg;

    if (user == conn->getUser()) {
        term.getStatusWindow()->error("You cannot send a message to yourself.");
        return STATUS_OK;
    }

    for (int i = 1; i < args.size(); i++) {
        msg += args[i];
        if (i < args.size() - 1) msg += ' ';
    }

    conn->sendMessage(user, msg);
    char logText[1024];
    sprintf(logText, "<%s> [@%s] %s", conn->getUser().c_str(), user.c_str(), msg.c_str());
    term.getMainWindow()->log(logText);

    return STATUS_OK;
}

int ChatClient::list(const vector<string> &args) {
    conn->requestList();
    return STATUS_OK;
}

int ChatClient::help(const vector<string> &args) {
    term.getMainWindow()->logFile(HELP_FILE, *term.getStatusWindow());
    return STATUS_OK;
}

int ChatClient::disconnect(const vector<string> &args) {
    conn->disconnect();
    conn = nullptr;
    term.getInputWindow()->setTag(InputWindow::DEFAULT_TAG);
    term.getStatusWindow()->clear();
    MainPtr main = term.getMainWindow();
    main->clearBuffer();
    main->clear();
    return STATUS_OK;
}

///
/// == Getters ==
///

bool ChatClient::isWaiting() {
    return waiting;
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

///
/// == Private methods ==
///

void ChatClient::showWelcome() {
    CenterPtr center = term.getCenterWindow();
    StatusPtr st = term.getStatusWindow();
    int y = center->printFile(TITLE_FILE, *st, 0);
    center->printFile(HELP_FILE, *st, y);
    center->refresh();
}
