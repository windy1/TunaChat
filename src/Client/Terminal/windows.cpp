//
// Created by Walker Crouse on 9/26/18.
//

#include "windows.h"
#include "Terminal.h"

///
/// == StatusWindow ==
///

StatusWindow::StatusWindow(Terminal &term) : Window(term, 1, term.getColumns(), 0, 0) {}

void StatusWindow::drawError(const string &err) {
    clear();
    colorOn(COLOR_PAIR_ERROR);
    addStr(0, 0, err);
    colorOff(COLOR_PAIR_ERROR);
}

///
/// == MainWindow ==
///

MainWindow::MainWindow(Terminal &term) :
        Window(term, term.getRows() - 2, term.getColumns(), 1, 0) {}

void MainWindow::draw() {
    drawDivider("\u2550", 0, 0);
    drawDivider("\u2550", rows - 1, 0);
}

///
/// == InputWindow ==
///

InputWindow::InputWindow(Terminal &term) : Window(term, 1, term.getColumns(), term.getRows() - 1, 0), tag("TunaChat") {}

void InputWindow::draw() {
    addStr(0, 0, "[" + tag + "] ");
}

void InputWindow::setTag(const string &tag) {
    this->tag = tag;
}

const string& InputWindow::getTag() const {
    return tag;
}
