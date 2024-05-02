#include "header.h"


// MOVE FUNCTIONS 

namespace move{
        void clear()                           { std::cout << "\x1b[2K\r"; };
        void clear(const std::string& newline) { std::cout << "\x1b[2K\r" <<  newline;  };
        
        void up()              { printf("\x1b[1A"); };
        void up(const int amt) { printf("\x1b[%iA", amt); };

        void down()              { printf("\x1b[1B"); };
        void down(const int amt) { printf("\x1b[%iB", amt); };

        void beginning() { printf("\r"); };

        void up_clear()        { std::cout << "\x1b[1A\x1b[2K\r"; };
        void up_clear(int amt) { while(amt) { std::cout << "\x1b[1A\x1b[2K\r"; amt--; } };

        void left()              { printf("\x1b[1D"); };
        void left(const int amt) { printf("\x1b[%iD", amt); };

        void right()              { printf("\x1b[1C"); };
        void right(const int amt) { printf("\x1b[%iC", amt); };


}

void sleep(const double& sec){    
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));    
}

void main_input(){
    std::string input;

    std::jthread log_thread(keylog, std::ref(input));
    log_thread.detach(); //daemon

    move::down(3);
    move::up(3);
    while(input.find("quit") == std::string::npos){


        std::cout << input << N << N;

        std::cout<< "// " << input; 
        //move::right(3+input.length());
        sleep(0.02);
        move::clear();
        std::cout<< "// " << input; 
        //move::right(3+input.length());
        //move::beginning();

        move::up_clear(2);

    }  
}

void keylog(std::string& into){
        while(1){
        char get = getch();
        switch(get){
            case ENTER:
                    into = "";
            break;  
            default:
                into.push_back(get);
        }
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