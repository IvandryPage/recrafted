#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <iostream>
#include <algorithm>

class InputHandler
{
    private:
        bool is_valid{};
        std::string input{};

        bool validateInput();
        void sanitizeInput();

    public:
        InputHandler();
        ~InputHandler();

        std::string getInput();
        void setInput(std::string input_param);

        bool getIsValid();
        void setIsValid(bool is_valid_param);

        std::string getPlayerInput();

};

#endif