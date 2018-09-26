//
// Created by Walker Crouse on 9/25/18.
//

#include "ChatClient.h"
#include "Terminal/InputWindow.h"
#include "Terminal/StatusWindow.h"
#include "Terminal/MainWindow.h"
#include "Command.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>

using std::stringstream;
using std::make_shared;

ChatClient::ChatClient() {
    commands.push_back(make_shared<Command>(this, "quit", &ChatClient::quit, "Usage: /quit"));
    commands.push_back(make_shared<Command>(
        this, "connect", &ChatClient::connect, "Usage: /connect <host> [port]", 2, 1));
}

int ChatClient::start() {
    Terminal term;

    MainPtr main = term.getMainWindow();
    StatusPtr statusWin = term.getStatusWindow();
    InputPtr input = term.getInputWindow();

    main->draw();
    main->refresh();

    while (status != STATUS_CLOSED) {
        input->clear();
        input->draw();
        input->refresh();

        string strIn;
        input->getStr(strIn);
        processInput(strIn);
    }

    return status;
}

int ChatClient::processInput(const string &in) {
    vector<string> args;
    parseArgs(in, args);
    for (auto &cmd : commands) {
        if (cmd->matches(in)) {
            return cmd->execute(args);
        }
    }

//    if (in.substr(0, strlen(CLIENT_CONN)) == CLIENT_CONN) {
//        StatusPtr statusWin = term.getStatusWindow();
//
//        vector<string> args;
//        parseArgs(in, args);
//
//        if (args.empty()) {
//            statusWin->drawError("Usage: /connect <host> [port]");
//            statusWin->refresh();
//            return status = STATUS_INVALID_ARG;
//        }
//
//        int port = 12000;
//        if (args.size() > 1) {
//            try {
//                port = stoi(args[1]);
//            } catch (...) {
//                statusWin->drawError("Invalid port number.");
//                statusWin->refresh();
//                return status = STATUS_INVALID_ARG;
//            }
//        }
//
//        return connect(args[0], port);
//    }

    return status = STATUS_UNKNOWN_CMD;
}

int ChatClient::quit(const vector<string> &args) {
    return status = STATUS_CLOSED;
}

int ChatClient::connect(const vector<string> &args) {
    StatusPtr statusWin = term.getStatusWindow();

    int port = 12000;
    if (args.size() > 1) {
        try {
            port = stoi(args[1]);
        } catch (...) {
            statusWin->drawError("Invalid port number.");
            statusWin->refresh();
            return status = STATUS_INVALID_ARG;
        }
    }

    char msg[1024];
    sprintf(msg, "Connecting to %s %d...", args[0].c_str(), port);
    statusWin->addStr(0, 0, msg);
    statusWin->refresh();

    sockaddr_in addr{};
    int socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        return status = STATUS_BAD_SOCKET;
    }

    return status;
}

Terminal& ChatClient::getTerminal() const {
    return (Terminal&) term;
}

int ChatClient::getStatus() const {
    return status;
}

void ChatClient::parseArgs(const string &cmd, vector<string> &vec) {
    vec.clear();
    size_t begin = cmd.find(' ');
    if (begin == string::npos || begin + 1 > cmd.size()) return;
    string args = cmd.substr(begin + 1);
    stringstream in(args);
    string arg;
    while (getline(in, arg, ' ')) {
        vec.push_back(arg);
    }
}
