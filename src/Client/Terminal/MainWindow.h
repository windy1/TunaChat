//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_MAINWINDOW_H
#define TUNACHAT_MAINWINDOW_H

#include "Window.h"

class MainWindow : public Window {
public:

    explicit MainWindow(Terminal &term);

    void draw();

};

#endif //TUNACHAT_MAINWINDOW_H
