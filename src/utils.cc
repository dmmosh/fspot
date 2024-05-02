#include "header.h"


// MOVE NAMESPACE
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

// HELPER FUNCTIONS

void sleep(const double& sec){    
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));    
}


// PLAYERS DEFAULTS

// default commands
void players::commands(){
    if (input == "quit"){ //quit
        type = false;
    }

    return;
};


// input and type initializer
players::players(std::string input, bool type): 
input(input), 
type(type),
log_thread(std::make_unique<std::jthread>(&players::keylog, this))
{

    return;
};
players::~players(){

    log_thread->join();
}

// CHARACTER INPUT  and keylog
// any subclass
void players::keylog(){
        while(type){
    
        
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
                commands();
                input = "";
            break;  
            default:
                if (input.length() <15)
                    input.push_back(buf);
        }
    }
}


// MAIN PLAYER CLASS

// main player constructor
main_player::main_player(): players("", true){


    //std::jthread log_thread(&main_player::keylog, this); //keylogging enabled
    
    move::down(3);
    move::up(3);
    while(type){

        
        std::cout << input << NEW << NEW;

        std::cout<< INVERT_ON << " // " << input << INVERT_OFF ; 
        //move::right(3+input.length());
        sleep(0.02);
        move::clear();
        std::cout<< INVERT_ON << " // " << input << INVERT_OFF; 
        //move::right(3+input.length());
        //move::beginning();

        move::up_clear(2);

    }  
    move::down(3);
}


void main_player::commands(){
    if (input == "quit"){ //quit
        type = false;
    } else if (input == "clear"){
        input = "dsjsjvdkdfklv";
    }

    return;
};


