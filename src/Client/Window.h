//
// Created by Walker Crouse on 9/25/18.
//

#ifndef TUNACHAT_WINDOW_H
#define TUNACHAT_WINDOW_H

#include "../tuna.h"
#include <curses.h>
#include <string>

using std::string;

class Window {
protected:

    WINDOW *win;
    int rows;
    int columns;
    int x;
    int y;

public:

    void drawHorizDivider(const string &ch, int y=0, int x=0, int colorPair=COLOR_PAIR_DIVIDER);

    void addStr(int y, int x, const string &str);

    void getStr(string &str);

    void refresh();

    int getRows() const;

    int getColumns() const;

    int getX() const;

    int getY() const;

};

#endif //TUNACHAT_WINDOW_H
