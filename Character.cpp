#include "Character.h"

Character::Character() = default;
Character::~Character() = default;
Character::Character(std::string name_param, std::string description_param)
{
    name = name_param;
    description = description_param;
}

void Character::speak(const std::string& line)
{
    std::cout << name << " : " << line << std::endl;
}