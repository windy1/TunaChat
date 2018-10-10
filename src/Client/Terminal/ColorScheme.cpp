//
// Created by Walker Crouse on 10/9/18.
//

#include "ColorScheme.h"

const ColorScheme ColorScheme::DARK;
const ColorScheme ColorScheme::LIGHT = ColorScheme(COLOR_PAIR_DIVIDER_L, COLOR_PAIR_TITLE_L, COLOR_PAIR_LIGHT);

ColorScheme::ColorScheme(int cpDivider, int cpTitle, int cpBg) : cpDivider(cpDivider), cpTitle(cpTitle), cpBg(cpBg) {}

int ColorScheme::getDivider() const {
    return cpDivider;
}

int ColorScheme::getTitle() const {
    return cpTitle;
}

int ColorScheme::getBackground() const {
    return cpBg;
}
