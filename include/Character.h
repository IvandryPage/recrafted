#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <iomanip>
#include "Animation.h"
#include "Color.h"

class Character {
public:
    Character();
    Character(std::string name_param, std::string description_param, 
              Color::ColorName character_color_param, bool is_first_param = true);
    ~Character();
    void speak(const std::string& line);
    void chat(const std::string& line);

private:
    std::string name_;
    std::string description_;
    Color::ColorName character_color_;
    bool is_first_encounter_;
};

#endif
