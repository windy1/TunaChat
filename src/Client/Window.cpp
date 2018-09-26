//
// Created by Walker Crouse on 9/25/18.
//

#include "Window.h"

void Window::drawHorizDivider(const string &ch, int y, int x, int colorPair) {
    string divider;
    for (int i = 0; i < columns; i++) divider += ch;
    wattron(win, COLOR_PAIR(colorPair));
    mvwaddstr(win, y, x, divider.c_str());
    wattroff(win, COLOR_PAIR(colorPair));
}

void Window::addStr(int y, int x, const string &str) {
    mvwaddstr(win, y, x, str.c_str());
}

void Window::getStr(string &str) {
    char s[1024];
    wgetstr(win, s);
    str = s;
}

void Window::refresh() {
    wrefresh(win);
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
