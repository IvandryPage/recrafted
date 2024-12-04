#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <iostream>
#include <algorithm>
#include <vector>

class InputHandler
{
    private:
        bool is_valid{};
        std::string input{};
        int sanitized_input{};

        bool validateInput(std::vector<std::string>& choices);
        void sanitizeInput(std::vector<std::string> choices);

    public:
        InputHandler();
        ~InputHandler();

        std::string getInput();
        void setSanitizedInput(int sanitized_input_param);
        int getSanitizedInput();

        bool getIsValid();
        void setIsValid(bool is_valid_param);

        std::string getPlayerInput(std::vector<std::string>& choices);

};

#endif