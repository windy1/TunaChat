//
// Created by Walker Crouse on 9/25/18.
//

#include "Terminal.h"
#include "windows.h"
#include <locale.h>

using std::make_shared;

///
/// == Terminal ==
///

Terminal::Terminal() {
    init();
    main = make_shared<MainWindow>(*this);
    status = make_shared<StatusWindow>(*this);
    input = make_shared<InputWindow>(*this);
}

Terminal::~Terminal() {
    endwin();
}

///
/// == Getters ==
///

MainPtr Terminal::getMainWindow() const {
    return main;
}

InputPtr Terminal::getInputWindow() const {
    return input;
}

StatusPtr Terminal::getStatusWindow() const {
    return status;
}

int Terminal::getRows() const {
    return rows;
}

int Terminal::getColumns() const {
    return columns;
}

///
/// == Private methods ==
///

void Terminal::init() {
    setlocale(LC_ALL, "");
    initscr();
    nonl();
    cbreak();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, rows, columns);

    start_color();
    init_pair(COLOR_PAIR_DIVIDER, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_ERROR, COLOR_WHITE, COLOR_RED);
}
