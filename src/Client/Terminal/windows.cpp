//
// Created by Walker Crouse on 9/26/18.
//

#include "windows.h"
#include "Terminal.h"
#include <fstream>

using std::ifstream;
using std::to_string;

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
/// == InputWindow ==
///

const string InputWindow::DEFAULT_TAG = "TunaChat";

InputWindow::InputWindow(Terminal &term)
    : Window(term, 2, term.getColumns(), term.getRows() - 2, 0), tagStr(DEFAULT_TAG) {}

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

///
/// == CenterWindow ==
///

CenterWindow::CenterWindow(Terminal &term) :
    Window(term, 21, 60, term.getRows()/3 - 21/2, term.getColumns()/2 - 60/2) {}

int CenterWindow::printFile(const string &fileName, StatusWindow &st, int y) {
    auto f = [&](const string &ln) {
        addStr(y, padding, ln);
        y++;
    };
    if (!tuna::fileFeed(fileName, f)) {
        st.error("Could not open file: " + fileName);
    }
    return y;
}
