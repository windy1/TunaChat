//
// Created by Walker Crouse on 9/25/18.
//

#include "InputWindow.h"

InputWindow::InputWindow(Terminal &term) : Window(), term(term) {
    win = newwin(rows = 2, columns = term.getColumns(), y = term.getRows() - 2, x = 0);
}

InputWindow::~InputWindow() {
    delwin(win);
}

void InputWindow::draw() {
    drawHorizDivider("\u2550");
    addStr(1, 0, "[TunaChat] ");
}
