//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_COMMAND_H
#define TUNACHAT_COMMAND_H

#include "ChatClient.h"
#include <string>

using std::string;

class Command {

    ChatClient *client;
    string name;
    int (ChatClient::*exe)(const vector<string>&);
    int maxArgs;
    int minArgs;
    string usage;

public:

    Command(
        ChatClient *client,
        const string &name,
        int (ChatClient::*exe)(const vector<string>&),
        const string &usage,
        int maxArgs = -1,
        int minArgs = -1);

    bool matches(const string &cmd);

    int execute(const vector<string> &args);

    const string& getName() const;

    static void parseArgs(const string &cmd, vector<string> &vec);
};

#endif //TUNACHAT_COMMAND_H
