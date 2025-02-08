#ifndef COLOR_H
#define COLOR_H
#pragma once

#include <iostream>

namespace Color {
    enum ColorName { 
        BLACK, WHITE, RAISIN_BLACK,
        TURKEY_RED, INDIGO, ORANGE,
        PEACH, VERDIGRIS, RUSTY_RED,
        AVOCADO, SKY_MAGENTA, SAPPHIRE,
        AUBURN, SAGE, ICE_BLUE,
        GRAY, VANILLA, CORAL,
        WOOD, MAUVINE, JADE,
        DEFAULT 
    };

    inline std::string getColorCode(ColorName color) {
        //RGB Value Color
        switch (color) {
            case BLACK: return "5;5;5";
            case WHITE: return "225;225;225";
            case RAISIN_BLACK: return "47;36;44";
            case TURKEY_RED: return "163;0;0";
            case INDIGO: return "0;71;119";
            case ORANGE: return "255;199;0";
            case PEACH: return "239;210;141";
            case VERDIGRIS: return "0;175;181";
            case RUSTY_RED: return "222;60;75";
            case AVOCADO: return "89;139;44";
            case SKY_MAGENTA: return "188;105;170";
            case SAPPHIRE: return "3;83;164";
            case AUBURN: return "172;57;49";
            case SAGE: return "168;186;154";
            case ICE_BLUE: return "155;243;240";
            case GRAY: return "156, 149, 131";
            case VANILLA: return "225;216;159";
            case CORAL: return "227;150;149";
            case WOOD: return "98;23;8";
            case MAUVINE: return "144;0;179";
            case JADE: return "38;169;108";
            default: return "255;255;255";
        }
    }
}

#endif