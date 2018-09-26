//
// Created by Walker Crouse on 9/25/18.
//

#include "InputWindow.h"
#include "Terminal.h"

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
