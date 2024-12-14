#include "Animation.h"

Animation::Animation(std::vector<std::string> frames_param, int frames_per_second_param, int duration_param)
{
    frames = frames_param;
    frames_per_second = frames_per_second_param;
    duration = duration_param;
}

Animation::~Animation() = default;

void Animation::type(std::string line)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for(auto& character : line)
    {
        std::cout << character << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(TYPE_SPEED));
    }
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Animation::type(std::string line, Color color)
{
    Animation::changeColor(color);
    Animation::type(line);
    Animation::resetColor();
}

void Animation::changeColor(Color color)
{
    std::cout << "\033[" << color << 'm';
}

void Animation::resetColor()
{
    std::cout << "\033[" << Color::WHITE << 'm';
}

void Animation::playAnimation()
{
    std::cout << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    for(std::string frame : frames)
    {
        std::cout << frame << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds( duration * 1000 / frames_per_second ));
    }
}