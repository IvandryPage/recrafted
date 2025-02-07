#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include "Animation.h"

class Timer
{
    private:
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        std::chrono::time_point<std::chrono::steady_clock> end_time;
        std::chrono::seconds total_elapsed_time{};
        bool timer_running {true};

    public:
        Timer();
        ~Timer();
        void startTimer();
        void stopTimer();
        int getTotalElapsedTime();
        void setTotalElapsedTime(std::chrono::seconds total_elapsed_time_param);
        void displayTime();
};

#endif