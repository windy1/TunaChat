//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_WINDOWS_H
#define TUNACHAT_WINDOWS_H

#include "Window.h"
#include <vector>

using std::vector;

class StatusWindow : public Window {
public:

    explicit StatusWindow(Terminal &term);

    void divider();

    /**
     * Clears the window and adds the specified error.
     *
     * @param err error to set
     */
    void error(const string &err);

};

class MainWindow : public Window {

    string logText;

public:

    explicit MainWindow(Terminal &term);

    void log(const string &text);

    void log(const string &user, const string &text);

};

class InputWindow : public Window {

    string tagStr;

public:

    explicit InputWindow(Terminal &term);

    void divider();

    void tag();

    void setTag(const string &tag);

    const string& getTag() const;

};

#endif //TUNACHAT_WINDOWS_H
