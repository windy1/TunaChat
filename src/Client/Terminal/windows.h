//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_WINDOWS_H
#define TUNACHAT_WINDOWS_H

#include "Window.h"
#include <vector>
#include <queue>
#include <functional>

using std::vector;
using std::queue;
using std::function;

class StatusWindow : public Window {
public:

    explicit StatusWindow(Terminal &term);

    void divider();

    void set(const string &text);

    /**
     * Clears the window and adds the specified error.
     *
     * @param err error to set
     */
    void error(const string &err);

};

class MainWindow : public Window {

    string logText;
    queue<string> logQueue;

    bool withFile(const string &fileName, StatusWindow &st, function<void (const string &ln)> f);

public:

    explicit MainWindow(Terminal &term);

    void debug(const string &text);

    void log(const string &text);

    void log(const string &user, const string &text);

    void flush();

    int printFile(const string &fileName, StatusWindow &st, int y=0);

    void logFile(const string &fileName, StatusWindow &st);

};

class InputWindow : public Window {

    string tagStr;

public:

    explicit InputWindow(Terminal &term);

    void reset();

    void divider();

    void tag();

    void setTag(const string &tag);

    const string& getTag() const;

};

#endif //TUNACHAT_WINDOWS_H
