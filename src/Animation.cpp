#include "../include/Animation.h"

Animation::Animation(std::vector<std::string> frames, int frames_per_second, int duration, 
                     bool cursor_focused, int row, int column)
    : frames_(std::move(frames)),
      frames_per_second_(frames_per_second),
      duration_(duration),
      cursor_focused_(cursor_focused),
      row_(row),
      column_(column)
{
    getPrevCursorPosition();
    playAnimation();
}

Animation::~Animation() {
    setCursorPosition(previous_column_, previous_row_);
}

void Animation::type(const std::string& line) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (char character : line) {
        std::cout << character << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(kTypeSpeed));
    }
    std::cout << '\n';

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void Animation::type(const std::string& line, Color::ColorName color) {
    withColor(color, [&]() { type(line); });
}

void Animation::focusCursor() {
    // FIXME : cursor_focused_ somehow set to false even if in the constructor and default value set to true
    if (!cursor_focused_) {
        setCursorPosition(row_, column_);
    }
}

void Animation::playAnimation() {
    int total_frames = frames_per_second_ * duration_;
    for (int i = 0; i < total_frames; ++i) {
        focusCursor();
        std::cout << frames_[current_frame_] << std::flush;
        current_frame_ = (current_frame_ + 1) % frames_.size();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frames_per_second_));
    }
}

void Animation::getPrevCursorPosition() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    previous_row_ = csbi.dwCursorPosition.Y;
    previous_column_ = csbi.dwCursorPosition.X;
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
    sscanf(buffer, "\033[%d;%dR", &previous_row_, &previous_column_);

    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    previous_row_--;
    previous_column_--;
#endif
}

void Animation::setCursorPosition(short row, short column) {
#ifdef _WIN32 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    COORD coord = {static_cast<SHORT>(column), static_cast<SHORT>(row)};
    SetConsoleCursorPosition(hConsole, coord);
#else
    std::cout << "\033[" << row << ";" << column << "H" << std::flush;
#endif
}