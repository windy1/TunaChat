//
// Created by Walker Crouse on 9/25/18.
//

#include "Terminal.h"
#include "windows.h"
#include "MainWindow.h"
#include <locale.h>

using std::make_shared;

///
/// == Terminal ==
///

Terminal::Terminal(const string &logFile) {
    init();
    main = make_shared<MainWindow>(*this, logFile);
    status = make_shared<StatusWindow>(*this);
    input = make_shared<InputWindow>(*this);
    center = make_shared<CenterWindow>(*this);
    userList = make_shared<UserListWindow>(*this);
}

Terminal::Terminal() : Terminal("") {}

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

CenterPtr Terminal::getCenterWindow() const {
    return center;
}

UserListPtr Terminal::getUserListWindow() const {
    return userList;
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
    //nonl();
    //cbreak();
    //intrflush(stdscr, FALSE);
    //keypad(stdscr, TRUE);
    getmaxyx(stdscr, rows, columns);

    start_color();
    init_pair(COLOR_PAIR_DIVIDER_H, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_ERROR, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_PAIR_TITLE, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_DIVIDER_V, COLOR_CYAN, COLOR_CYAN);
    init_pair(COLOR_PAIR_DEBUG, COLOR_BLACK, COLOR_WHITE);
}
