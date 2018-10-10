//
// Created by Walker Crouse on 9/25/18.
//

#include "Window.h"

using std::to_string;

///
/// == Window ==
///

Window::Window(Terminal &term, int rows, int columns, int y, int x) : term(term), visible(true) {
    win = newwin(this->rows = rows, this->columns = columns, y, x);
}

Window::~Window() {
    delwin(win);
}

///
/// == Methods ==
///

void Window::drawHDiv(const string &ch, int y, int x, int colorPair) {
    string divider;
    for (int i = 0; i < columns - x; i++) divider += ch;
    if (colorPair != -1) colorOn(colorPair);
    addStr(y, x, divider);
    if (colorPair != -1) colorOff(colorPair);
}

void Window::drawVDiv(const string &ch, int y, int x, int colorPair) {
    if (colorPair != -1) colorOn(colorPair);
    for (int row = y; row < rows; row++) {
        addStr(row, x, ch);
    }
    if (colorPair != -1) colorOff(colorPair);
}

void Window::colorOn(int colorPair) {
    wattron(win, COLOR_PAIR(colorPair));
}

void Window::colorOff(int colorPair) {
    wattroff(win, COLOR_PAIR(colorPair));
}

///
/// == Wrapper methods ==
///

void Window::addStr(int y, int x, const string &str) {
    mvwaddstr(win, y, x, str.c_str());
}

void Window::getStr(string &str) {
    char s[1024];
    wgetstr(win, s);
    str = s;
}

void Window::refresh() {
    if (visible) wrefresh(win);
}

void Window::clear() {
    wclear(win);
}

void Window::scrollOk(bool scroll) {
    scrollok(win, scroll);
}

void Window::box(char ch) {
    ::box(win, ch, ch);
}

void Window::resize(int rows, int columns) {
    wresize(win, this->rows = rows, this->columns = columns);
}

void Window::move(int y, int x) {
    wmove(win, this->y = y, this->x = x);
}

void Window::background(int colorPair) {
    wbkgd(win, COLOR_PAIR(colorPair));
}

///
/// == Getters ==
///

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
