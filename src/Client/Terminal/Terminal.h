//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_CLI_H
#define TUNACHAT_CLI_H

#include "Window.h"
#include <memory>

using std::shared_ptr;

class MainWindow;
class InputWindow;
class StatusWindow;

typedef shared_ptr<MainWindow> MainPtr;
typedef shared_ptr<InputWindow> InputPtr;
typedef shared_ptr<StatusWindow> StatusPtr;

class Terminal {

    MainPtr main;
    InputPtr input;
    StatusPtr status;

    int rows;
    int columns;

    void init();

public:

    Terminal();

    ~Terminal();

    MainPtr getMainWindow() const;

    InputPtr getInputWindow() const;

    StatusPtr getStatusWindow() const;

    int getRows() const;

    int getColumns() const;

};

#endif //TUNACHAT_CLI_H
