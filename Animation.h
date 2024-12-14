#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Color.h"

#define TYPE_SPEED 50

class Animation
{
    
    private:
        std::vector<std::string> frames;
        int frames_per_second;
        int duration;
        int current_frame;

    public:
        Animation(std::vector<std::string> frames_param, int frames_per_second_param, int duration_param);
        ~Animation();

        static void type(std::string line);
        static void type(std::string line, Color color);
        static void changeColor(Color color);
        static void resetColor();
        void playAnimation();
};

#endif