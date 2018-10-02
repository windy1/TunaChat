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
class CenterWindow;

typedef shared_ptr<MainWindow> MainPtr;
typedef shared_ptr<InputWindow> InputPtr;
typedef shared_ptr<StatusWindow> StatusPtr;
typedef shared_ptr<CenterWindow> CenterPtr;

class Terminal {

    MainPtr main;
    InputPtr input;
    StatusPtr status;
    CenterPtr center;

    int rows;
    int columns;

    /**
     * Initializes ncurses.
     */
    void init();

public:

    Terminal();

    ~Terminal();

    /**
     * Returns the main window for the Terminal, this is the window where chat is displayed.
     *
     * @return main window
     */
    MainPtr getMainWindow() const;

    /**
     * Returns the input window for the Terminal, this is the window where the user submits input.
     *
     * @return input window
     */
    InputPtr getInputWindow() const;

    /**
     * Returns the status window for the Terminal, this is where the client will display status information.
     *
     * @return status window
     */
    StatusPtr getStatusWindow() const;

    /**
     * Returns the window in the middle of the Terminal that displays the welcome message.
     *
     * @return center window
     */
    CenterPtr getCenterWindow() const;

    /**
     * Returns the total rows on the screen.
     *
     * @return total rows on screen
     */
    int getRows() const;

    /**
     * Returns the total columns on the screen.
     *
     * @return total columns on screen
     */
    int getColumns() const;

};

#endif //TUNACHAT_CLI_H
