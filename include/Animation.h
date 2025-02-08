#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Color.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

constexpr int kTypeSpeed {30};

class Animation {
public:
    Animation(std::vector<std::string> frames, int frames_per_second, int duration,
                bool cursor_focused = true, int row = 0, int column = 0);
    ~Animation();

    static void type(const std::string& line);
    static void type(const std::string& line, Color::ColorName color);

    static inline void changeColor(Color::ColorName color) {
        std::cout << "\033[38;2;" << Color::getColorCode(color) << 'm' << std::flush;
    }

    static inline void resetColor() {
        std::cout << "\033[38;2;" << Color::getColorCode(Color::DEFAULT) << 'm' << std::flush;
    }

    template <typename Func>
    static inline void withColor(Color::ColorName color, Func func) {
        static_assert(std::is_invocable_v<Func>, "requires a function");
        
        changeColor(color);
        func();
        resetColor();
    }

    void setCursorPosition(short row, short column);
    void getPrevCursorPosition();
    void focusCursor();
    void playAnimation();

private:
    std::vector<std::string> frames_;
    int current_frame_ {0};

    short frames_per_second_ {0};
    short duration_ {0}; // in seconds

    unsigned short row_ {0};        
    unsigned short column_ {0};
    unsigned short previous_row_ {0};
    unsigned short previous_column_ {0};

    bool cursor_focused_ {};
};

#endif