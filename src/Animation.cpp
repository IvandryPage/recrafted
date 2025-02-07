#include "../include/Animation.h"

Animation::Animation(std::vector<std::string> frames_param, int frames_per_second_param, int duration_param, bool cursor_focused_param, int row_param, int column_param)
{
    frames = frames_param;
    frames_per_second = frames_per_second_param;
    duration = duration_param;
    cursor_focused = cursor_focused_param;
    row = row_param;
    column = column_param;
    getPrevCursorPosition();
    playAnimation();
}

Animation::~Animation()
{
    setCursorPosition(prev_col, prev_row);
}

void Animation::type(std::string line)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    for(auto& character : line)
    {
        std::cout << character << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(TYPE_SPEED));
    }
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
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
        setCursorPosition(row, column);
    }
}

void Animation::getPrevCursorPosition() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    prev_row = csbi.dwCursorPosition.Y;
    prev_col = csbi.dwCursorPosition.X;
#else
    struct termios orig_term, raw_term;
    tcgetattr(STDIN_FILENO, &orig_term);
    raw_term = orig_term;
    raw_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw_term);

    std::cout << "\033[6n";
    std::cout.flush();

    char buffer[16];
    read(STDIN_FILENO, buffer, sizeof(buffer));
    sscanf(buffer, "\033[%d;%dR", &prev_row, &prev_col);

    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    prev_row--;
    prev_col--;
#endif
}

void Animation::setCursorPosition(short row_param, short column_param) {
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    COORD coord = {static_cast<SHORT>(column_param), static_cast<SHORT>(row_param)};
    SetConsoleCursorPosition(hConsole, coord);
#else
    std::cout << "\033[" << row_param << ";" << column_param << "H" << std::flush;
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

#if defined(__linux__) || defined(__APPLE__)
void Animation::setRawMode(bool enable) {
    static struct termios old_tio, new_tio;

    if (enable) {
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    }
}
#endif

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