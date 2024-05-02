#include <vector>
#include <memory>
#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>



// MACROS
// SMALL FUNCTIONS HERE
#define NEW '\n'
#define ENTER 10

#define BOLD_ON "\033[1m"
#define BOLD_OFF "\033[0m"
#define INVERT_ON "\033[;7;1m"
#define INVERT_OFF "\033[0m"
#define DEFAULT_ON "\033[39;49m"
#define DEFAULT_OFF "\033[0m"
#define LINE_ON "\x1b[41;37;1m\033[;7;1m"
#define LINE_OFF "\x1b[K\x1b[0m"

#define ERROR(x) std::cout << LINE_ON << "FATAL ERROR:\n\t" << x << LINE_OFF; exit(0)


namespace move{
    void clear();
    void clear(const std::string& newline);

    void up();
    void up(const int amt);

    void down();
    void down(const int amt);

    void beginning();

    void up_clear();
    void up_clear(int amt);

    void left();
    void left(const int amt);

    void right();
    void right(const int amt);
};



void sleep(const double& sec);
void main_input();



// HELPER FUNCTIONS

// CHARACTER INPUT  and keylog
void keylog(std::string& into);
char getch();