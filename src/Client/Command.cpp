//
// Created by Walker Crouse on 9/26/18.
//

#include "ChatClient.h"
#include "Command.h"
#include "Terminal/StatusWindow.h"
#include <sstream>

using std::stringstream;

Command::Command(
    ChatClient *client,
    const string &name,
    int (ChatClient::*exe)(const vector<string>&),
    const string &usage,
    int maxArgs,
    int minArgs) :
    client(client),
    name(name),
    exe(exe),
    usage(usage),
    maxArgs(maxArgs),
    minArgs(minArgs) {}

bool Command::matches(const string &cmd) {
    return cmd.substr(0, cmd.find(' ')) == '/' + name;
}

int Command::execute(const vector<string> &args) {
    bool invalid = false;
    if (maxArgs != -1 && args.size() > maxArgs) invalid = true;
    if (minArgs != -1 && args.size() < minArgs) invalid = true;
    if (invalid) {
        StatusPtr statusWin = client->getTerminal().getStatusWindow();
        statusWin->drawError(usage);
        statusWin->refresh();
        return STATUS_INVALID_ARG;
    }
    return (client->*exe)(args);
}

const string& Command::getName() const {
    return name;
}

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