//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_STATUSWINDOW_H
#define TUNACHAT_STATUSWINDOW_H

#include "Window.h"

class StatusWindow : public Window {
public:

    explicit StatusWindow(Terminal &term);

    void drawError(const string &err);

};

#endif //TUNACHAT_STATUSWINDOW_H
