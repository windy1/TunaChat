//
// Created by Walker Crouse on 9/26/18.
//

#include "tests.h"
#include "ChatClient.h"
#include "Command.h"

int runTests() {
    int failed = 0;
    if (!test_ChatClient_parseArgs()) failed++;
    if (!test_host2ip()) failed++;
    if (!test_Preference_keyExists()) failed++;
    return failed;
}

bool test_ChatClient_parseArgs() {
    printf("== test_ChatClient_parseArgs ==\n");
    string cmd = "/test hello world foo bar";
    vector<string> args;
    Command::parseArgs(cmd, args);
    return args[0] == "hello" && args[1] == "world" && args[2] == "foo" && args[3] == "bar";
}

bool test_host2ip() {
    printf("== test_host2_ip ==\n");
    string host = "google.com";
    string ip;
    tuna::host2ip(host, ip);
    printf("ip = %s\n", ip.c_str());
    return true;
}

bool test_Preference_keyExists() {
    printf("== test_Preference_keyExists ==");
    Preferences prefs;
    prefs("foo", "bar");
    return !prefs.keyExists("bar") && prefs.keyExists("foo");
}
