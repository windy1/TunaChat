//
// Created by Walker Crouse on 9/26/18.
//

#include "StatusWindow.h"
#include "Terminal.h"

StatusWindow::StatusWindow(Terminal &term) : Window(term, 1, term.getColumns(), 0, 0) {}

void StatusWindow::drawError(const string &err) {
    clear();
    colorOn(COLOR_PAIR_ERROR);
    addStr(0, 0, err);
    colorOff(COLOR_PAIR_ERROR);
}
