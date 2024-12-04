#include "InputHandler.h"

InputHandler::InputHandler() = default;
InputHandler::~InputHandler() = default;

bool InputHandler::getIsValid() { return is_valid; }
void InputHandler::setIsValid(bool is_valid_param) { is_valid = is_valid_param; }

std::string InputHandler::getInput() { return input; }
void InputHandler::setInput(std::string input_param) { input = input_param; }

std::string InputHandler::getPlayerInput() 
{
    std::getline(std::cin, input);
    
    if(!validateInput())
    {
        std::cout << "Input is not valid!" << std::endl;
        this->getPlayerInput();
    }
    
    sanitizeInput();

    return input;
}

bool InputHandler::validateInput()
{
    if(input.empty())
    {
        is_valid = false;
        return false;
    }

    is_valid = true;
    return true;
}

void InputHandler::sanitizeInput()
{
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
}