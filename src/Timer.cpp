#include "../include/Timer.h"

void Timer::stopTimer() {
    timer_running = false;
    end_time = std::chrono::steady_clock::now();
    total_elapsed_time += std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
}

void Timer::displayTime() {
    int hours = getTotalElapsedTime() / 3600;
    int minutes = (getTotalElapsedTime() % 3600) / 60;
    int seconds = getTotalElapsedTime() % 60;

    std::cout << "You've played this game for "
              << std::right << std::setw(2) << std::setfill('0') << hours << ":"
              << std::setw(2) << std::setfill('0') << minutes << ":"
              << std::setw(2) << std::setfill('0') << seconds << "\n";
}