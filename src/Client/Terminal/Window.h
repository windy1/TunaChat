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

    WINDOW *win;

protected:

    Terminal &term;

    int rows;
    int columns;
    int x;
    int y;
    bool visible;

public:

    Window(Terminal &term, int rows, int columns, int y, int x);

    virtual ~Window();

    /**
     * Draws a horizontal divider across the window using the specified string, starting at the specified coordinates
     * using the specified color pair.
     *
     * @param ch divider character
     * @param y start coordinate
     * @param x start coordinate
     * @param colorPair registered color pair
     */
    void drawDivider(const string &ch, int y = 0, int x = 0, int colorPair = COLOR_PAIR_DIVIDER);

    /**
     * Toggles on the color attribute for the specified color pair.
     *
     * @param colorPair color pair to enable
     */
    void colorOn(int colorPair);

    /**
     * Toggles off the color attribute for the specified color pair.
     *
     * @param colorPair color pair to enable
     */
    void colorOff(int colorPair);

    /**
     * Returns the amount of rows in the window.
     *
     * @return rows in window
     */
    int getRows() const;

    /**
     * Returns the amount of columns in the window.
     *
     * @return columns in window
     */
    int getColumns() const;

    /**
     * Returns this Window's x-coordinate on the screen, which is mapped to the upper-left most corner.
     *
     * @return x coordinate
     */
    int getX() const;

    /**
     * Returns this Window's y-coordinate on the screen, which is mapped to the upper-left most corner.
     *
     * @return y coordinate
     */
    int getY() const;

    ///
    /// == Wrapper methods ==
    /// The following methods are wrappers for coresponding ncurses functions.
    ///

    void addStr(int y, int x, const string &str);
    void getStr(string &str);
    void refresh();
    void clear();
    void scrollOk(bool scroll);

    /// ======================

};

#endif //TUNACHAT_WINDOW_H
