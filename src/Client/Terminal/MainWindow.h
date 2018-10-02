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

    void debug(const string &text);

    void log(const string &text);

    void log(const string &user, const string &text);

    void flush();

    int printFile(const string &fileName, StatusWindow &st, int y=0);

    void logFile(const string &fileName, StatusWindow &st);

};

#endif //TUNACHAT_MAINWINDOW_H
