#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

class InputHandler {
public:
    InputHandler() = default;
    ~InputHandler() = default;

    void getPlayerInput(const std::vector<std::string>& choices);
    char getKey();

    inline const std::string getInput() const { 
        return input_; 
    }
    
    inline const short getSanitizedInput() const {
        return sanitized_input_;
    }

    inline void setSanitizedInput(short sanitized_input) {
        sanitized_input_ = sanitized_input;
    }

    inline const bool getIsValid() const {
        return is_valid_;
    }

    inline void setIsValid(bool is_valid) {
        is_valid_ = is_valid;
    }

private:
    std::string input_;
    short sanitized_input_{0};
    bool is_valid_{false};

    bool validateInput(const std::vector<std::string>& choices);
    void sanitizeInput(const std::vector<std::string>& choices);
};

#endif