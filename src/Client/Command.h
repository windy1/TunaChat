//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_COMMAND_H
#define TUNACHAT_COMMAND_H

#include "ChatClient.h"
#include <string>

using std::string;
using std::make_shared;

/**
 * Typedef declaration for ChatClient member function to execute Commands.
 */
typedef int (ChatClient::*CmdExe)(const vector<string>&);

class Command {

    ChatClient *client;
    string name;
    CmdExe exe;
    int maxArgs;
    int minArgs;
    string usage;
    bool connReq;
    bool authReq;

public:

    Command(
        ChatClient *client,
        const string &name,
        CmdExe exe,
        const string &usage,
        int maxArgs = -1,
        int minArgs = -1,
        bool connReq=false,
        bool authReq=false);

    /**
     * Returns true if the specified full input command matches this command and should be executed.
     *
     * @param cmd command to check
     * @return true if command should be executed
     */
    bool matches(const string &cmd);

    /**
     * Invokes this Command's executor.
     *
     * @param args command line arguments
     * @return status code
     */
    int execute(const vector<string> &args);

    /**
     * Returns the client this command belongs to.
     *
     * @return client of command
     */
    ChatClient* getClient() const;

    /**
     * Returns the name of the command.
     *
     * @return command name
     */
    const string& getName() const;

    /**
     * Returns a pointer to the ChatClient member-function that is responsible for executing this command.
     *
     * @return command executor
     */
    CmdExe getExecutor() const;

    /**
     * Returns the maximum amount of arguments permitted in this command (-1 for unlimited).
     *
     * @return max arguments
     */
    int getMaxArgs() const;

    /**
     * Returns the minimum amount of arguments permitted in this commnad (-1 for no minimum).
     *
     * @return min arguments
     */
    int getMinArgs() const;

    /**
     * Returns a string describing how to use this command.
     *
     * @return command usage
     */
    const string& getUsage() const;

    /**
     * Returns true if the client must be connected to a server for this command to execute.
     *
     * @return true if connection required
     */
    bool isConnectionRequired() const;

    /**
     * Returns true if the client must be authenticated with a server for this command to execute.
     *
     * @return true if auth required
     */
    bool isAuthenticationRequired() const;

    /**
     * Discards the command name from the specified input and parses all subsequent arguments into a vector.
     *
     * @param cmd command to parse
     * @param vec vector to put arguments in
     */
    static void parseArgs(const string &cmd, vector<string> &vec);

};

#endif //TUNACHAT_COMMAND_H
