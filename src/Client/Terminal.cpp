//
// Created by Walker Crouse on 9/25/18.
//

#include "Terminal.h"
#include "InputWindow.h"
#include <locale.h>

using std::make_unique;

Terminal::Terminal() : Window() {
    init();
    win = stdscr;
    x = y = 0;
    input = make_unique<InputWindow>(*this);

    input->draw();
    input->refresh();

    string in;
    input->getStr(in);
}

Terminal::~Terminal() {
    endwin();
}

void Terminal::init() {
    setlocale(LC_ALL, "");
    initscr();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, rows, columns);

    start_color();
    init_pair(COLOR_PAIR_DIVIDER, COLOR_CYAN, COLOR_BLACK);
}
