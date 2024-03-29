//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_CHATCLIENT_H
#define TUNACHAT_CHATCLIENT_H

#include "../tuna.h"
#include "Terminal/Terminal.h"
#include "Preferences.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Command;
class ServerConn;

typedef shared_ptr<Command> CmdPtr;
typedef shared_ptr<ServerConn> ServerConnPtr;

class ChatClient {

    static const string TITLE_FILE;
    static const string HELP_FILE;
    static const string LOG_FILE;
    static const string CONF_FILE;

    Terminal term;
    vector<CmdPtr> commands;
    ServerConnPtr conn;
    Preferences prefs;
    bool waiting = false;
    int status = STATUS_OK;

    /**
     * Initializes the commands for this client.
     */
    void initCommands();

    /**
     * Displays the welcome message on the center window.
     */
    void showWelcome();

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
     * Sends a request to the server to send back the user list.
     *
     * @param args command line arguments
     * @return status code
     */
    int list(const vector<string> &args);

    /**
     * Prints the help file.
     *
     * @param args command line arguments
     * @return status code
     */
    int help(const vector<string> &args);

    /**
     * Disconnects from the current server, if one.
     *
     * @param args command line arguments
     * @return status code
     */
    int disconnect(const vector<string> &args);

    /**
     * Clears the current log output in the main window.
     *
     * @param args command line arguments
     * @return status code
     */
    int clear(const vector<string> &args);

    /**
     * Sets the preference to the specified value as given in the argument.
     *
     * @param args command line arguments
     * @return status code
     */
    int preference(const vector<string> &args);

    /**
     * Lists currently set preferences.
     *
     * @param args command line arguments
     * @return status code
     */
    int preferences(const vector<string> &args);

    /**
     * Returns true if the client (main thread) is currently waiting for user input. This method is used to ensure there
     * are no concurrent modifications to the Terminal.
     *
     * @return true if client is waiting for user input
     */
    bool isWaiting();

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
     * Returns the preferences of the client.
     *
     * @return preferences reference
     */
    Preferences& getPreferences() const;

    /**
     * Returns the client's current status code.
     *
     * @return status code
     */
    int getStatus() const;

};

#endif //TUNACHAT_CHATCLIENT_H
