//
// Created by Walker Crouse on 10/9/18.
//

#ifndef TUNACHAT_COLORSCHEME_H
#define TUNACHAT_COLORSCHEME_H

#include "../../tuna.h"

class ColorScheme {

    int cpDivider;
    int cpTitle;
    int cpBg;

public:

    static const ColorScheme DARK;
    static const ColorScheme LIGHT;

    explicit ColorScheme(
        int cpDivider = COLOR_PAIR_DIVIDER,
        int cpTitle = COLOR_PAIR_TITLE,
        int cpBg = COLOR_PAIR_DARK);

    int getDivider() const;

    int getTitle() const;

    int getBackground() const;

};

#endif //TUNACHAT_COLORSCHEME_H
