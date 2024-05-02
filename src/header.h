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

#define AUTH_URL "https://accounts.spotify.com/authorize" 
#define TOKEN_URL "https://accounts.spotify.com/api/token" 
#define BASE_URL "https://api.spotify.com/v1/" 

#define BOLD_ON "\033[1m"
#define BOLD_OFF "\033[0m"
#define INVERT_ON "\033[;7;1m"
#define INVERT_OFF "\033[0m"
#define DEFAULT_ON "\033[39;49m"
#define DEFAULT_OFF "\033[0m"
#define LINE_ON "\x1b[41;37;1m\033[;7;1m"
#define LINE_OFF "\x1b[K\x1b[0m"

// fatal error message
#define ERROR(x) std::cout << LINE_ON << "FATAL ERROR:" << LINE_OFF << NEW <<  LINE_ON << "   " << x << LINE_OFF; exit(0)


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


// PLAYER BASE CLASS
class players{
    public:
    std::string input;
    bool type;

    players(std::string input, bool type);
    ~players();

    void commands();

};


// MAIN PLAYER SUBCLASS
class main_player: public players{
    public:
    main_player();
    ~main_player();

    void commands();

};

// HELPER FUNCTIONS

// CHARACTER INPUT  and keylog

template <class T> 
void keylog(T* parent){
        while(parent->type){

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
        

        if (!buf) return;

        switch(buf){
            case ENTER:
                parent->commands();
            break;  
            default:
                parent->input.push_back(buf);
        }
    }
}
