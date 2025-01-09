#ifndef COLOR_H
#define COLOR_H

#include <iostream>

namespace Color
{
    enum ColorName { BLACK, WHITE, RED, BLUE, GREEN, GREY, DEFAULT };

    std::string getColorCode(ColorName color);
}

#endif