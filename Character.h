#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include "Animation.h"

class Character
{
    
    private:
        std::string name;
        std::string description;

    public:
        Character();
        Character(std::string name_param, std::string description_param);
        ~Character();
        void speak(std::string line);

};

#endif
