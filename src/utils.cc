#include "header.h"


// MOVE FUNCTIONS 

namespace move{
        void line()                           { std::cout << "\x1b[2K\r"; };
        void line(const std::string& newline) { std::cout << "\x1b[2K\r" <<  newline;  };
        
        void up()        { std::cout << "\x1b[1A"; };
        void up(int amt) { while(amt) { up(); amt--; } };
        
        void clear()        { up(); clear(); };
        void clear(int amt) { while(amt) { clear(); amt--; } };

}

void sleep(const double& sec){    
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));    
}

void main_input(){
    std::string input = "";
    while(1 && input.find("quit") == std::string::npos){

        char get = getch();
        if (get) input.push_back(get);

        std::cout << input << N << N;

        std::cout<< "\n// " << input; 
        sleep(0.2);
        move::clear();
        std::cout<< "// " << input; 

        move::up();
        move::clear(2);

    }  
}

char getch() {
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
        return (buf);
}