//
// Created by Walker Crouse on 9/26/18.
//

#include "MainWindow.h"
#include "Terminal.h"

MainWindow::MainWindow(Terminal &term) :
    Window(term, term.getRows() - 2, term.getColumns(), 1, 0) {}

void MainWindow::draw() {
    drawDivider("\u2550", 0, 0);
    drawDivider("\u2550", rows - 1, 0);
}
