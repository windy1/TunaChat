//
// Created by Walker Crouse on 10/2/18.
//

#include "MainWindow.h"
#include "Terminal.h"
#include <fstream>
#include <ctime>

using std::ifstream;
using std::ofstream;
using std::ios_base;
using std::asctime;
using std::localtime;

///
/// == MainWindow ==
///

MainWindow::MainWindow(Terminal &term, const string &logFile)
        : logFile(logFile), Window(term, term.getRows() - 4, term.getColumns() - 2, 2, 1) {
    scrollOk(true);
}

///
/// == Methods ==
///

void MainWindow::debug(const string &text) {
    buffer.push("[debug] " + text);
}

void MainWindow::log(const string &text) {
    buffer.push(text);
}

void MainWindow::log(const string &user, const string &text) {
    char str[1024];
    sprintf(str, "<%s> %s", user.c_str(), text.c_str());
    log(str);
}

void MainWindow::flush(StatusWindow &st) {
    string outStr;
    while (!buffer.empty()) {
        if (!logText.empty()) logText += '\n';
        string str = buffer.front();
        logText += str;

        time_t time = std::time(nullptr);
        string ts = asctime(localtime(&time));
        ts = ts.substr(0, ts.size() - 1);
        char os[1024];
        sprintf(os, "[%s] %s\n", ts.c_str(), str.c_str());
        outStr += os;

        buffer.pop();
        clear();
        addStr(rows - 1, 0, logText);
    }

    if (!logFile.empty()) {
        ofstream out(logFile, ios_base::app);
        if (!out) {
            st.error("Could not log to file.");
            return;
        }
        out << outStr;
        out.close();
    }
}

void MainWindow::clearBuffer() {
    buffer = queue<string>();
    logText = "";
}

void MainWindow::cat(const string &fileName, StatusWindow &st) {
    auto f = [&](const string &ln) { log(ln); };
    if (!tuna::fileFeed(fileName, f)) {
        st.error("Could not open file: " + fileName);
    }
}
