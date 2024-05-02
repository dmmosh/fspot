#include "header.h"


// MOVE FUNCTIONS 
move_obj::move_obj() {};

void move_obj::line()                           { std::cout << "\x1b[2K\r"; }
void move_obj::line(const std::string& newline) { std::cout << "\x1b[2K\r" <<  newline;  };

void move_obj::up()        { std::cout << "\x1b[1A"; };
void move_obj::up(int amt) { while(amt) { this->up(); amt--; } };

void move_obj::clear()        { this->up(); this->clear(); }
void move_obj::clear(int amt) { while(amt) { this->clear(); amt--; } }

void sleep(const double& sec){    
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));    
}

void main_input(std::string& input){
    std::unique_ptr<move_obj> move = std::make_unique<move_obj>();
    std::cout << N*4;
    while(1 && input.find("quit") == std::string::npos){
        keylog(input);
        std::cout << input << N << N;

        std::cout<< "\n// " << input; 
        sleep(0.2);
        move->clear();
        std::cout<< "// " << input; 

        move->up();
        move->clear(2);

    }  
}

// keylogging function
void keylog(std::string& into) {
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

        if (buf){
            into.push_back(buf);
        }
}

