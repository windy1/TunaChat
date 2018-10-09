//
// Created by Walker Crouse on 9/25/18.
//

#include "ChatClient.h"
#include "Terminal/windows.h"
#include "Terminal/MainWindow.h"
#include "ServerConn.h"
#include "Command.h"
#include <sstream>
#include <fstream>

using std::stringstream;
using std::make_shared;
using std::ifstream;

///
/// == Statics ==
///

const string ChatClient::TITLE_FILE = "../files/title.txt";
const string ChatClient::HELP_FILE = "../files/help.txt";
const string ChatClient::LOG_FILE = "../files/log.txt";
const string ChatClient::CONF_FILE = "../files/conf.txt";

///
/// == ChatClient ==
///

ChatClient::ChatClient() {
    Preferences::load(prefs, CONF_FILE);
    term = Terminal(prefs.getBool("LogFile") ? LOG_FILE : "");
    commands = {
        make_shared<Command>(this, "quit", &ChatClient::quit, "Usage: /quit"),
        make_shared<Command>(this, "connect", &ChatClient::connect, "Usage: /connect [host] [port]", 2, -1),
        make_shared<Command>(this, "auth", &ChatClient::authenticate, "Usage: /auth [user] [pass]", 2, -1, true),
        make_shared<Command>(this, "tell", &ChatClient::tell, "Usage: /tell <user> <message>", -1, 2, true, true),
        make_shared<Command>(this, "list", &ChatClient::list, "Usage: /list", -1, -1, true, true),
        make_shared<Command>(this, "disconnect", &ChatClient::disconnect, "Usage: /disconnect", -1, -1, true),
        make_shared<Command>(this, "clear", &ChatClient::clear, "Usage: /clear"),
        make_shared<Command>(this, "pref", &ChatClient::preference, "Usage: /pref <key> <value>", 2, 2),
        make_shared<Command>(this, "prefs", &ChatClient::preferences, "Usage: /prefs"),
        make_shared<Command>(this, "help", &ChatClient::help, "Usage: /help")
    };
}

///
/// == Methods ==
///

int ChatClient::start() {
    MainPtr main = term.getMainWindow();
    StatusPtr st = term.getStatusWindow();
    InputPtr input = term.getInputWindow();
    CenterPtr center = term.getCenterWindow();
    UserListPtr userList = term.getUserListWindow();
    while (status != STATUS_CLOSED) {
        refresh();

        st->divider();
        st->refresh();

        main->flush(*st);
        main->refresh();

        if (userList->isOpened()) {
            userList->divider();
            userList->refresh();
        }

        if (conn == nullptr) showWelcome();

        input->reset();
        input->refresh();

        waiting = true;
        string strIn;
        input->getStr(strIn);
        waiting = false;

        status = processInput(strIn);
    }

    return status;
}

int ChatClient::processInput(const string &in) {
    vector<string> args;
    Command::parseArgs(in, args);
    for (auto &cmd : commands) {
        if (cmd->matches(in)) {
            return cmd->execute(args);
        }
    }
    term.getStatusWindow()->error("Unknown command.");
    return STATUS_UNKNOWN_CMD;
}

///
/// == Command executors ==
///

int ChatClient::quit(const vector<string> &args) {
    if (conn != nullptr) {
        conn->disconnect();
        conn = nullptr;
    }
    return STATUS_CLOSED;
}

int ChatClient::connect(const vector<string> &args) {
    StatusPtr st = term.getStatusWindow();
    CenterPtr center = term.getCenterWindow();

    string host;
    Command::getArg(host, args, 0, prefs("DefaultHost"));
    int port = prefs.getInt("DefaultPort");

    char msg[100];
    sprintf(msg, "Connecting to %s %d...", host.c_str(), port);
    st->set(msg);
    st->divider();
    st->refresh();

    center->clear();
    center->refresh();
    if ((conn = make_shared<ServerConn>(*this, host, port))->getStatus() != STATUS_OK) {
        conn = nullptr;
        showWelcome();
    }

    return STATUS_OK;
}

int ChatClient::authenticate(const vector<string> &args) {
    string user;
    string pwd;
    Command::getArg(user, args, 0, prefs("DefaultUser"));
    Command::getArg(pwd, args, 1, prefs("DefaultPass"));
    conn->authenticate(user, pwd);
    return STATUS_OK;
}

int ChatClient::tell(const vector<string> &args) {
    string user = args[0];
    string msg;

    if (user == conn->getUser()) {
        term.getStatusWindow()->error("You cannot send a message to yourself.");
        return STATUS_OK;
    }

    for (int i = 1; i < args.size(); i++) {
        msg += args[i];
        if (i < args.size() - 1) msg += ' ';
    }

    conn->sendMessage(user, msg);
    char logText[1024];
    sprintf(logText, "<%s> [@%s] %s", conn->getUser().c_str(), user.c_str(), msg.c_str());
    term.getMainWindow()->log(logText);

    return STATUS_OK;
}

int ChatClient::list(const vector<string> &args) {
    UserListPtr userList = term.getUserListWindow();
    MainPtr main = term.getMainWindow();
    if (userList->isOpened()) {
        userList->close(*main);
        userList->refresh();
    } else {
        userList->open(*main);
        conn->requestList();
    }
    return STATUS_OK;
}

int ChatClient::help(const vector<string> &args) {
    term.getMainWindow()->cat(HELP_FILE, *term.getStatusWindow());
    return STATUS_OK;
}

int ChatClient::disconnect(const vector<string> &args) {
    conn->disconnect();
    conn = nullptr;
    term.getInputWindow()->setTag(InputWindow::DEFAULT_TAG);
    term.getStatusWindow()->clear();
    MainPtr main = term.getMainWindow();
    main->clearBuffer();
    main->clear();

    UserListPtr userList = term.getUserListWindow();
    if (userList->isOpened()) {
        userList->close(*main);
        userList->refresh();
    }

    return STATUS_OK;
}

int ChatClient::clear(const vector<string> &args) {
    MainPtr main = term.getMainWindow();
    main->clearBuffer();
    main->clear();
    main->refresh();
    return STATUS_OK;
}

int ChatClient::preference(const vector<string> &args) {
    StatusPtr st = term.getStatusWindow();
    if (prefs(args[0], args[1])) {
        st->set("Preference set.");
        prefs.save();
        return STATUS_OK;
    }
    st->error("No preference with key: " + args[0]);
    return STATUS_OK;
}

int ChatClient::preferences(const vector<string> &args) {
    term.getMainWindow()->log(to_string(prefs));
    return STATUS_OK;
}

///
/// == Getters ==
///

bool ChatClient::isWaiting() {
    return waiting;
}

Terminal& ChatClient::getTerminal() const {
    return (Terminal&) term;
}

const vector<CmdPtr>& ChatClient::getCommands() const {
    return commands;
}

ServerConnPtr ChatClient::getConnection() const {
    return conn;
}

Preferences& ChatClient::getPreferences() const {
    return (Preferences&) prefs;
}

int ChatClient::getStatus() const {
    return status;
}

///
/// == Private methods ==
///

void ChatClient::showWelcome() {
    CenterPtr center = term.getCenterWindow();
    center->colorOn(COLOR_PAIR_TITLE);
    StatusPtr st = term.getStatusWindow();
    int y = center->printFile(TITLE_FILE, *st, 0);
    center->colorOff(COLOR_PAIR_TITLE);
    center->printFile(HELP_FILE, *st, y);
    center->refresh();
}
