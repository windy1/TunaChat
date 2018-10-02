//
// Created by Walker Crouse on 10/2/18.
//

#ifndef TUNACHAT_MAINWINDOW_H
#define TUNACHAT_MAINWINDOW_H

#include "windows.h"

class MainWindow : public Window {

    string logText;
    queue<string> logQueue;

public:

    explicit MainWindow(Terminal &term);

    /**
     * Prints a debug message to the console.
     *
     * @param text to print
     */
    void debug(const string &text);

    /**
     * Prints a message to the console.
     *
     * @param text
     */
    void log(const string &text);

    /**
     * Prints a message to the console from the specified user.
     *
     * @param user sending message
     * @param text message text
     */
    void log(const string &user, const string &text);

    /**
     * Flushes the log queue, clears, and adds the text to the screen.
     */
    void flush();

    /**
     * Prints the contents of the specified file to the console.
     *
     * @param fileName file name to print contents of
     * @param st status window
     */
    void logFile(const string &fileName, StatusWindow &st);

};

#endif //TUNACHAT_MAINWINDOW_H
