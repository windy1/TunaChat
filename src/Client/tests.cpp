//
// Created by Walker Crouse on 9/26/18.
//

#include "tests.h"
#include "ChatClient.h"
#include <cstdio>

int runTests() {
    int failed = 0;
    if (!test_ChatClient_parseArgs()) failed++;
    return failed;
}

bool test_ChatClient_parseArgs() {
    printf("== test_ChatClient_parseArgs ==\n");
    string cmd = "/test hello world foo bar";
    vector<string> args;
    ChatClient::parseArgs(cmd, args);
    return args[0] == "hello" && args[1] == "world" && args[2] == "foo" && args[3] == "bar";
}
