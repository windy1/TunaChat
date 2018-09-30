//
// Created by Walker Crouse on 9/26/18.
//

#include "windows.h"
#include "Terminal.h"

///
/// == StatusWindow ==
///

StatusWindow::StatusWindow(Terminal &term) : Window(term, 2, term.getColumns(), 0, 0) {}

void StatusWindow::divider() {
    drawDivider("\u2550", 1, 0);
}

void StatusWindow::set(const string &text) {
    clear();
    addStr(0, 0, text);
}

void StatusWindow::error(const string &err) {
    clear();
    colorOn(COLOR_PAIR_ERROR);
    addStr(0, 0, err);
    colorOff(COLOR_PAIR_ERROR);
}

///
/// == MainWindow ==
///

MainWindow::MainWindow(Terminal &term) :
        Window(term, term.getRows() - 4, term.getColumns() - 2, 2, 1) {
    scrollOk(true);
}

void MainWindow::debug(const string &text) {
    logQueue.push("[debug] " + text);
}

void MainWindow::log(const string &text) {
    logQueue.push(text);
}

void MainWindow::log(const string &user, const string &text) {
    char str[1024];
    sprintf(str, "<%s> %s", user.c_str(), text.c_str());
    log(str);
}

void MainWindow::flush() {
    while (!logQueue.empty()) {
        if (!logText.empty()) logText += '\n';
        logText += logQueue.front();
        logQueue.pop();
        clear();
        addStr(rows - 1, 0, logText);
    }
}

///
/// == InputWindow ==
///

InputWindow::InputWindow(Terminal &term) : Window(term, 2, term.getColumns(), term.getRows() - 2, 0), tagStr("TunaChat") {}

void InputWindow::reset() {
    clear();
    divider();
    tag();
}

void InputWindow::divider() {
    drawDivider("\u2550", 0, 0);
}

void InputWindow::tag() {
    addStr(1, 0, "[" + tagStr + "] ");
}

void InputWindow::setTag(const string &tag) {
    this->tagStr = tag;
}

const string& InputWindow::getTag() const {
    return tagStr;
}
