//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_CHATCLIENT_H
#define TUNACHAT_CHATCLIENT_H

#include "../tuna.h"
#include "Terminal/Terminal.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Command;

typedef shared_ptr<Command> CmdPtr;

class ChatClient {

    Terminal term;
    int status = STATUS_OK;
    vector<CmdPtr> commands;

public:

    ChatClient();

    int start();

    int processInput(const string &in);

    int quit(const vector<string> &args);

    int connect(const vector<string> &args);

    Terminal& getTerminal() const;

    int getStatus() const;

    static void parseArgs(const string &cmd, vector<string> &vec);

};

#endif //TUNACHAT_CHATCLIENT_H
