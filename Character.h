#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <iomanip>
#include "Animation.h"
#include "Color.h"

class Character
{
    
    private:
        std::string name;
        std::string description;
        Color::ColorName character_color;

    public:
        Character();
        Character(std::string name_param, std::string description_param, Color::ColorName character_color_param);
        ~Character();
        void speak(std::string line);

};

#endif
