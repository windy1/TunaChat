//
// Created by Walker Crouse on 9/25/18.
//

#include "Window.h"

Window::Window(Terminal &term, int rows, int columns, int y, int x) : term(term) {
    win = newwin(this->rows = rows, this->columns = columns, y, x);
}

Window::~Window() {
    delwin(win);
}

void Window::drawDivider(const string &ch, int y, int x, int colorPair) {
    string divider;
    for (int i = 0; i < columns; i++) divider += ch;
    colorOn(colorPair);
    addStr(y, x, divider);
    colorOff(colorPair);
}

void Window::addStr(int y, int x, const string &str) {
    mvwaddstr(win, y, x, str.c_str());
}

void Window::getStr(string &str) {
    char s[1024];
    wgetstr(win, s);
    str = s;
}

void Window::print(const string &str) {
    wprintw(win, str.c_str());
}

void Window::colorOn(int colorPair) {
    wattron(win, COLOR_PAIR(colorPair));
}

void Window::colorOff(int colorPair) {
    wattroff(win, COLOR_PAIR(colorPair));
}

void Window::refresh() {
    wrefresh(win);
}

void Window::clear() {
    wclear(win);
}

int Window::getRows() const {
    return rows;
}

int Window::getColumns() const {
    return columns;
}

int Window::getX() const {
    return x;
}

int Window::getY() const {
    return y;
}