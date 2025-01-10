#ifndef COLOR_H
#define COLOR_H

#include <iostream>

namespace Color
{
    enum ColorName { 
        BLACK, WHITE, RAISIN_BLACK,
        TURKEY_RED, INDIGO, ORANGE,
        PEACH, VERDIGRIS, RUSTY_RED,
        AVOCADO, SKY_MAGENTA, SAPPHIRE,
        AUBURN, SAGE, ICE_BLUE,
        GRAY, VANILLA, CORAL,
        WOOD, MAUVINE, JADE,
        DEFAULT };

    std::string getColorCode(ColorName color);
}

#endif