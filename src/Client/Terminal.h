//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_CLI_H
#define TUNACHAT_CLI_H

#include "Window.h"
#include <memory>

using std::unique_ptr;

class InputWindow;

class Terminal : public Window {

    unique_ptr<InputWindow> input;

    void init();

public:

    Terminal();

    ~Terminal();

};

#endif //TUNACHAT_CLI_H
