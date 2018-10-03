//
// Created by Walker Crouse on 10/2/18.
//

#include "MainWindow.h"
#include "Terminal.h"
#include <fstream>

using std::ifstream;

///
/// == MainWindow ==
///

MainWindow::MainWindow(Terminal &term) :
        Window(term, term.getRows() - 4, term.getColumns() - 2, 2, 1) {
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

void MainWindow::flush() {
    while (!buffer.empty()) {
        if (!logText.empty()) logText += '\n';
        logText += buffer.front();
        buffer.pop();
        clear();
        addStr(rows - 1, 0, logText);
    }
}

void MainWindow::clearBuffer() {
    buffer = queue<string>();
}

void MainWindow::logFile(const string &fileName, StatusWindow &st) {
    auto f = [&](const string &ln) { log(ln); };
    if (!tuna::fileFeed(fileName, f)) {
        st.error("Could not open file: " + fileName);
    }
}
