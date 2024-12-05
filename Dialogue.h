#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <iostream>

class Dialogue
{
    public:
        int character_index;    
        std::string line;

        Dialogue(int character_index_param, std::string line_param);
};

#endif