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
        bool is_first_encounter;

    public:
        Character();
        Character(std::string name_param, std::string description_param, Color::ColorName character_color_param, bool is_first_param = true);
        ~Character();
        void speak(std::string line);
        void chat(std::string line);

};

#endif
