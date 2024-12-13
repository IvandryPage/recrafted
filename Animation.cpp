#include "Animation.h"

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