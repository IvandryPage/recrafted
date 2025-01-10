#include "Timer.h"

Timer::Timer() = default;
Timer::~Timer() = default;

int Timer::getTotalElapsedTime() { return total_elapsed_time.count(); }

void Timer::startTimer()
{
    start_time = std::chrono::steady_clock::now();
}

void Timer::stopTimer()
{
    timer_running = false;
    end_time = std::chrono::steady_clock::now();
    total_elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    displayTime();
}

void Timer::displayTime() 
{
    int hours = getTotalElapsedTime() / 3600;
    int minutes = (getTotalElapsedTime() % 3600) / 60;
    int seconds = getTotalElapsedTime() % 60;

    std::cout << "You've played this game for " << hours << ":" << minutes << ":" << seconds << std::endl;
}