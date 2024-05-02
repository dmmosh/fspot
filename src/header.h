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
#define N '\n'

namespace move{
    void line();
    void line(const std::string& newline);

    void up();
    void up(int amt);

    void clear();
    void clear(int amt);

    void left();
    void left(int amt);

    void right();
    void right(int amt);
};


void sleep(const double& sec);
void main_input();

// HELPER FUNCTIONS

// CHARACTER INPUT 
char keylog();

