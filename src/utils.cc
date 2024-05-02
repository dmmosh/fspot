#include "header.h"



void move::line()                           { std::cout << "\x1b[2K\r"; }
void move::line(const std::string& newline) { std::cout << "\x1b[2K\r" <<  newline;  };

void move::up()        { std::cout << "\x1b[1A"; };
void move::up(int amt) { while(amt) { move::up(); amt--; } };

void move::clear()        { move::up(); move::clear(); }
void move::clear(int amt) { while(amt) { move::clear(); amt--; } }

void sleep(const double& sec){    
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));    
}

void main_input(){
    std::string input;
    
    while(1){
        input.push_back(keylog());
        std::cout << input << N << N;

        std::cout<< "\n// " << input; 
        sleep(0.2);
        move::clear();
        std::cout<< "// " << input; 

        move::up();
        move::clear(2);
    }  
}

// keylogging function
char keylog() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");

        return buf;
}

