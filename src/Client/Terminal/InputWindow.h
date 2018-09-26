//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_INPUTWINDOW_H
#define TUNACHAT_INPUTWINDOW_H

#include "Window.h"

class InputWindow : public Window {

    string tag;

public:

    explicit InputWindow(Terminal &term);

    void draw();

    void setTag(const string &tag);

    const string& getTag() const;

};

#endif //TUNACHAT_INPUTWINDOW_H
