#include "../include/Character.h"

Character::Character() = default;
Character::~Character() = default;
Character::Character(std::string name_param, std::string description_param, Color::ColorName character_color_param, bool is_first_param)
{
    name = name_param;
    description = description_param;
    character_color = character_color_param;
    is_first_encounter = is_first_param;
}

void Character::speak(std::string line)
{
    if(!(name.find("Narrator") != std::string::npos))
    {
        Animation::changeColor(character_color);
        if(!is_first_encounter)
            std::cout << std::left << std::setw(10) << std::setfill(' ') << name;
        else
        {
            std::cout << std::left << std::setw(10) << std::setfill(' ') << "???";
            is_first_encounter = false;
        }
        Animation::resetColor();
        std::cout << " : " << std::flush;
        Animation::type(line);
    }
    else
    {
        std::cout << std::endl;
        Animation::type(line);
        std::cout << std::endl;
    } 
    
}

void Character::chat(std::string line)
{
    if(!(name.find("Narrator") != std::string::npos))
    {
        Animation::changeColor(character_color);

        if(name.find("Eva") != std::string::npos)
            std::cout << std::right << std::setw(50) << std::setfill(' ') << name;
        else
            std::cout << std::left << std::setw(10) << std::setfill(' ') << name;

        Animation::resetColor();
        std::cout << " : \n" << std::flush;
        
        if(name.find("Eva") != std::string::npos)
            std::cout << std::setw(20) << std::setfill(' ') << " ";

        Animation::type(line);
        std::cout << "\n";
    }
    else
    {
        std::cout << "\n\n";
        Animation::type(line);
        std::cout << std::endl;
    } 
}