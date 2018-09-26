//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_WINDOWS_H
#define TUNACHAT_WINDOWS_H

#include "Window.h"

class StatusWindow : public Window {
public:

    explicit StatusWindow(Terminal &term);

    /**
     * Clears the window and adds the specified error.
     *
     * @param err error to set
     */
    void drawError(const string &err);

};

class MainWindow : public Window {
public:

    explicit MainWindow(Terminal &term);

    void draw();

};

class InputWindow : public Window {

    string tag;

public:

    explicit InputWindow(Terminal &term);

    void draw();

    void setTag(const string &tag);

    const string& getTag() const;

};

#endif //TUNACHAT_WINDOWS_H
