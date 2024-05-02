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
        void line()                           { std::cout << "\x1b[2K\r"; };
        void line(const std::string& newline) { std::cout << "\x1b[2K\r" <<  newline;  };
        
        void up()        { std::cout << "\x1b[1A"; };
        void up(int amt) { while(amt) { up(); amt--; } };
        
        void clear()        { up(); clear(); }
        void clear(int amt) { while(amt) { clear(); amt--; } }

}



void sleep(const double& sec);
void main_input();

// HELPER FUNCTIONS

// CHARACTER INPUT 
char getch();