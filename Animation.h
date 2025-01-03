#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Color.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif


#define TYPE_SPEED 50

class Animation
{
    
    private:
        std::vector<std::string> frames;
        int frames_per_second {};
        int duration {};
        int current_frame {};
        short row{};
        short column{};
        bool cursor_focused {};

    public:
        Animation(std::vector<std::string> frames_param, int frames_per_second_param, int duration_param, bool cursor_focused_param = false, int row = 0, int column = 0);
        ~Animation();

        static void type(std::string line);
        static void type(std::string line, Color::ColorName color);
        static void changeColor(Color::ColorName color);
        static void resetColor();
        void focusCursor();
        void playAnimation();
        void setCursorPosition();
        void getCursorPosition();
        void setRawMode(bool enable);
        
};

#endif