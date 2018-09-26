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
class ServerConn;

typedef shared_ptr<Command> CmdPtr;
typedef shared_ptr<ServerConn> ServerConnPtr;

class ChatClient {

    Terminal term;
    int status = STATUS_OK;
    vector<CmdPtr> commands;
    ServerConnPtr conn;

public:

    ChatClient();

    /**
     * Starts the client, initializes the interface, and begins taking input.
     *
     * @return status code
     */
    int start();

    /**
     * Processes the specified string as input and attempts to execute a command.
     *
     * @param in input string
     * @return status code result
     */
    int processInput(const string &in);

    /**
     * Closes the client.
     *
     * @param args command line arguments
     * @return status code
     */
    int quit(const vector<string> &args);

    /**
     * Attempts to connect to a specified host on a specified port. Creates and assigns the active connection of this
     * client.
     *
     * @param args command line arguments
     * @return status code
     */
    int connect(const vector<string> &args);

    /**
     * Attempts to authenticate if there is an open connection with the specified username and password.
     *
     * @param args command line arguments
     * @return status code
     */
    int authenticate(const vector<string> &args);

    /**
     * Attempts to send a message to the specified user with the specified text.
     *
     * @param args command line arguments
     * @return status code
     */
    int tell(const vector<string> &args);

    /**
     * Returns the Terminal interface for this client.
     *
     * @return Terminal
     */
    Terminal& getTerminal() const;

    /**
     * Returns a vector of all registered Commands on the client.
     *
     * @return registered commands for client
     */
    const vector<CmdPtr>& getCommands() const;

    /**
     * Returns the active connection if there is one, nullptr otherwise.
     *
     * @return active connection
     */
    ServerConnPtr getConnection() const;

    /**
     * Returns the client's current status code.
     *
     * @return status code
     */
    int getStatus() const;

};

#endif //TUNACHAT_CHATCLIENT_H
