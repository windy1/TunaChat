//
// Created by Walker Crouse on 9/26/18.
//

#include "ChatClient.h"
#include "Command.h"
#include "Terminal/windows.h"
#include "ServerConn.h"
#include <sstream>

using std::stringstream;

///
/// == Command ==
///

Command::Command(
    ChatClient *client,
    const string &name,
    CmdExe exe,
    const string &usage,
    int maxArgs,
    int minArgs,
    bool connReq,
    bool authReq) :
    client(client),
    name(name),
    exe(exe),
    usage(usage),
    maxArgs(maxArgs),
    minArgs(minArgs),
    connReq(connReq),
    authReq(authReq) {}

///
/// == Methods ==
///

bool Command::matches(const string &cmd) {
    return cmd.substr(0, cmd.find(' ')) == '/' + name;
}

int Command::execute(const vector<string> &args) {
    bool invalid = false;
    if (maxArgs != -1 && args.size() > maxArgs) invalid = true;
    if (minArgs != -1 && args.size() < minArgs) invalid = true;

    StatusPtr statusWin = client->getTerminal().getStatusWindow();
    if (invalid) {
        statusWin->error(usage);
        return STATUS_INVALID_ARG;
    }

    ServerConnPtr conn = client->getConnection();
    if (connReq && conn == nullptr) {
        statusWin->error("You are not connected to a server.");
        return STATUS_INVALID_ARG;
    }

    if (authReq && conn != nullptr && !conn->isAuthenticated()) {
        statusWin->error("You are not authenticated with the server.");
        return STATUS_INVALID_ARG;
    }

    return (client->*exe)(args);
}

///
/// == Getters ==
///

ChatClient* Command::getClient() const {
    return client;
}

const string& Command::getName() const {
    return name;
}

CmdExe Command::getExecutor() const {
    return exe;
}

int Command::getMaxArgs() const {
    return maxArgs;
}

int Command::getMinArgs() const {
    return minArgs;
}

const string& Command::getUsage() const {
    return usage;
}

bool Command::isConnectionRequired() const {
    return connReq;
}

bool Command::isAuthenticationRequired() const {
    return authReq;
}

///
/// == Static methods ==
///

void Command::parseArgs(const string &cmd, vector<string> &vec) {
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

void Command::getArg(string &arg, const vector<string> &args, int i, const string &def) {
    if (args.size() > i) arg = args[i];
    else arg = def;
}
