#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include "Animation.h"

class Timer{
public:
    Timer() = default;
    ~Timer() = default;
    
    void stopTimer();
    void displayTime();
    
    inline void startTimer() {
        start_time = std::chrono::steady_clock::now();
    }

    inline const int getTotalElapsedTime() const {
        return total_elapsed_time.count();
    }
    
    inline void setTotalElapsedTime(std::chrono::seconds total_elapsed_time_param) {
        total_elapsed_time = total_elapsed_time_param;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    std::chrono::seconds total_elapsed_time{0};
    bool timer_running {true};
};

#endif