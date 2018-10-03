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

struct Err;

class StatusWindow : public Window {
public:

    explicit StatusWindow(Terminal &term);

    void divider();
    void set(const string &text);
    void error(const string &err);
    void error(const Err &err);

};

class InputWindow : public Window {

    string tagStr;

public:

    static const string DEFAULT_TAG;

    explicit InputWindow(Terminal &term);

    void reset();
    void divider();
    void tag();

    void setTag(const string &tag);
    const string& getTag() const;

};

class CenterWindow : public Window {

    int padding = 1;

public:

    explicit CenterWindow(Terminal &term);

    int printFile(const string &fileName, StatusWindow &st, int y);

};

struct Err {
    string text;
    int code;
};

#endif //TUNACHAT_WINDOWS_H
