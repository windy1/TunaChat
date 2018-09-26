//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_INPUTWINDOW_H
#define TUNACHAT_INPUTWINDOW_H

#include "Window.h"

class InputWindow : public Window {
public:

    explicit InputWindow(Terminal &term);

    void draw();

};

#endif //TUNACHAT_INPUTWINDOW_H
