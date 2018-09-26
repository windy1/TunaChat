//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_INPUTWINDOW_H
#define TUNACHAT_INPUTWINDOW_H

#include "Terminal.h"
#include <thread>

using std::thread;

class InputWindow : public Window {

    Terminal &term;

public:

    explicit InputWindow(Terminal &term);

    ~InputWindow();

    void draw();

};

#endif //TUNACHAT_INPUTWINDOW_H
