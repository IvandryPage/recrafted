#include "Color.h"

std::string Color::getColorCode(ColorName color)
{
    //RGB Value Color
    switch(color)
    {
        case BLACK: return "0;0;0";
        case WHITE: return "255;255;255";
        case RED: return "255;0;0";
        case BLUE: return "0;0;255";
        case GREEN: return "0;255;0";
    }
    
    return "255;255;255";
}