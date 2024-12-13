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

void Animation::changeColor(Color color)
{
    std::cout << "\033[" << static_cast<int>(color) << 'm';
}

void Animation::resetColor()
{
    std::cout << "\033[" << static_cast<int>(Color::WHITE) << 'm';
}