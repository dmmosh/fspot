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

void move_line();
void move_line(const std::string& newline);
void move_up();
void move_up(int amt);
void move_clear();
void move_clear(int amt);



void sleep(const double& sec);
void main_input();

// HELPER FUNCTIONS

// CHARACTER INPUT 
char keylog();

