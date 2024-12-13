#include "Animation.h"

void Animation::type(std::string line)
{
    for(auto& character : line)
    {
        std::cout << character << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(TYPE_SPEED));
    }
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}