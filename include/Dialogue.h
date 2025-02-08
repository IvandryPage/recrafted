#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <iostream>

class Dialogue {
public:
    int character_index_;    
    std::string line_;
    std::string monologue_;

    Dialogue(int character_index, std::string line, std::string monologue = "")
        : character_index_(character_index), line_(line), monologue_(monologue) {}
};

#endif