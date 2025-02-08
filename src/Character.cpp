#include "../include/Character.h"

Character::Character() = default;
Character::~Character() = default;
Character::Character(std::string name, std::string description, Color::ColorName character_color, bool is_first)
    : name_(name), description_(description), character_color_(character_color), is_first_encounter_(is_first) {}

void Character::speak(const std::string& line) {
    if (name_.find("Narrator") != std::string::npos) {
        std::cout << '\n';
        Animation::type(line);
        std::cout << '\n';
        return;
    } 

    Animation::withColor(character_color_, [&]() {
        std::cout << std::left << std::setw(10) << std::setfill(' ') 
            << (is_first_encounter_ ? "???" : name_);

        if(is_first_encounter_) {
            is_first_encounter_ = false;
        }
    });
    
    std::cout << " : " << std::flush;
    Animation::type(line);
}

void Character::chat(const std::string& line) {
    if(name_.find("Narrator") != std::string::npos) {
        std::cout << "\n\n";
        Animation::type(line);
        std::cout << "\n";
        return;
    }

    bool is_eva = name_.find("Eva") != std::string::npos;

    Animation::withColor(character_color_, [&]() {
        short indentation = (is_eva) ? 50 : 10;
        auto alignment = (is_eva) ? std::right : std::left;
        std::cout << alignment << std::setw(indentation) << std::setfill(' ') << name_;
    });

    std::cout << " : \n" << std::flush; 
    if(is_eva) {
        std::cout << std::setw(20) << std::setfill(' ') << " ";
    }

    Animation::type(line);
    std::cout << "\n";
}