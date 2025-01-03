#include "Animation.h"

Animation::Animation(std::vector<std::string> frames_param, int frames_per_second_param, int duration_param, bool cursor_focused_param, int row_param, int column_param)
{
    frames = frames_param;
    frames_per_second = frames_per_second_param;
    duration = duration_param;
    cursor_focused = cursor_focused_param;
    row = row_param;
    column = column_param;
    playAnimation();
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

void Animation::type(std::string line, Color::ColorName color)
{
    Animation::changeColor(color);
    Animation::type(line);
    Animation::resetColor();
}

void Animation::changeColor(Color::ColorName color)
{
    std::cout << "\033[38;2;" << Color::getColorCode(color) << 'm';
}

void Animation::resetColor()
{
    std::cout << "\033[38;2;" << Color::getColorCode(Color::DEFAULT) << 'm';
}

void Animation::focusCursor() {
    if (cursor_focused) {
        setCursorPosition();
    }
}

void Animation::setCursorPosition() {
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    COORD coord = {static_cast<SHORT>(column), static_cast<SHORT>(row)};
    SetConsoleCursorPosition(hConsole, coord);
#else
    std::cout << "\033[" << row << ";" << column << "H";
#endif
}

void Animation::playAnimation() {
    int total_frames = frames_per_second * duration;
    for (int i = 0; i < total_frames; ++i) {
        focusCursor();
        std::cout << frames[current_frame] << std::flush;
        current_frame = (current_frame + 1) % frames.size();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frames_per_second));
    }
}

void Animation::setRawMode(bool enable) {
#if defined(__linux__) || defined(__APPLE__)
    static struct termios old_tio, new_tio;

    if (enable) {
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    }
#endif
}

#ifdef _WIN32
void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif