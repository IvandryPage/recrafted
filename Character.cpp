#include "Character.h"

Character::Character() = default;
Character::~Character() = default;
Character::Character(std::string name_param, std::string description_param, Color::ColorName character_color_param)
{
    name = name_param;
    description = description_param;
    character_color = character_color_param;
}

void Character::speak(std::string line)
{
    Animation::changeColor(character_color);
    std::cout << std::left << std::setw(10) << name << " : " << std::flush;
    Animation::resetColor();
    
    Animation::type(line);
}