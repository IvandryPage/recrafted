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

        void getPlayerInput(std::vector<std::string>& choices);

        char getKey();

};

#endif