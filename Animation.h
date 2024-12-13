#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#define TYPE_SPEED 50

class Animation
{
    
    private:
        std::vector<std::string> frames;
        int frames_per_second;
        int current_frame;

    public:
        Animation(std::vector<std::string> frames_param, int frames_per_second);
        ~Animation();

        static void type(std::string line);
        void playAnimation();
};

#endif