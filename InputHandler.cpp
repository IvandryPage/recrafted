#include "InputHandler.h"

InputHandler::InputHandler() = default;
InputHandler::~InputHandler() = default;

bool InputHandler::getIsValid() { return is_valid; }
void InputHandler::setIsValid(bool is_valid_param) { is_valid = is_valid_param; }

std::string InputHandler::getInput() { return input; }
int InputHandler::getSanitizedInput() { return sanitized_input; }
void InputHandler::setSanitizedInput(int sanitized_input_param) { sanitized_input = sanitized_input_param; }

void InputHandler::getPlayerInput(std::vector<std::string>& choices) 
{
    std::getline(std::cin, input);
    
    if(!validateInput(choices))
    {
        std::cout << "Input is not valid!" << std::endl;
        this->getPlayerInput(choices);
    }
}

bool InputHandler::validateInput(std::vector<std::string>& choices)
{
    if(input.empty())
    {
        is_valid = false;
        return false;
    }

    if(std::isdigit(input[0]))
        setSanitizedInput((std::stoi(input.substr(0, 1)) - 1));
    else
        sanitizeInput(choices);

    if (sanitized_input >= std::size(choices) || sanitized_input < 0)
    {
        is_valid = false;
        return false;
    }

    is_valid = true;
    return true;
}

void InputHandler::sanitizeInput(std::vector<std::string> choices)
{
    sanitized_input = -1;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    for(std::string& choice : choices)
        std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
    
    for(int i {}; i < std::size(choices); i++ )
    {

        if(choices[i].find(input) != std::string::npos)
        {
            bool is_unique = true;
            for(int j {};  j < std::size(choices); j++)
            {                
                if(j != i && choices[j].find(input) != std::string::npos)
                {
                    is_unique = false;
                    break;
                }
            }

            if(is_unique)
            {
                sanitized_input = i;
                break;
            }
        }

    }
}