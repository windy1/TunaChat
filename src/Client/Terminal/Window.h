//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_WINDOW_H
#define TUNACHAT_WINDOW_H

#include "../../tuna.h"
#include <curses.h>
#include <string>

using std::string;

class Terminal;

class Window {
protected:

    WINDOW *win;

    Terminal &term;

    int rows;
    int columns;
    int x;
    int y;

public:

    Window(Terminal &term, int rows, int columns, int y, int x);

    virtual ~Window();

    void drawDivider(const string &ch, int y = 0, int x = 0, int colorPair = COLOR_PAIR_DIVIDER);

    void addStr(int y, int x, const string &str);

    void getStr(string &str);

    void print(const string &str);

    void colorOn(int colorPair);

    void colorOff(int colorPair);

    void refresh();

    void clear();

    int getRows() const;

    int getColumns() const;

    int getX() const;

    int getY() const;

};

#endif //TUNACHAT_WINDOW_H
