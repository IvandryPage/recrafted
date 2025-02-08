#include "../include/InputHandler.h"

void InputHandler::getPlayerInput(const std::vector<std::string>& choices) {
    std::cout << "\n> Ketikkan pilihan Anda: ";
    std::getline(std::cin, input_);
    
    if (!validateInput(choices)) {
        std::cout << "Input is not valid!\n";
        this->getPlayerInput(choices);
    }
}

bool InputHandler::validateInput(const std::vector<std::string>& choices) {
    if (input_.empty()) {
        is_valid_ = false;
        return is_valid_;
    }

    if(std::isdigit(input_[0])) {
        setSanitizedInput((std::stoi(input_.substr(0, 1)) - 1));
    } else {
        sanitizeInput(choices);
    }

    is_valid_ = (sanitized_input_ >= 0 && sanitized_input_ < std::size(choices));
    return is_valid_;
}

void InputHandler::sanitizeInput(const std::vector<std::string>& choices) {
    std::transform(input_.begin(), input_.end(), input_.begin(), ::tolower);

    sanitized_input_ = -1;
    
    for (size_t i = 0; i < std::size(choices); i++ ) {
        std::string choice_lower = choices[i];
        std::transform(choice_lower.begin(), choice_lower.end(), choice_lower.begin(), ::tolower);

        if(choices[i].find(input_) != std::string::npos) {
            sanitized_input_ = i;
            break;
        }
    }
}

#if defined(__linux__) || defined(__APPLE__)
char InputHandler::getKey() {
    struct termios oldt, newt;
    char ch;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif