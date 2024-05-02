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

// PLAYERS DEFAULTS

void players::commands(){
    if (this->input == "quit"){
        this->type = false;
        exit(0);
    }

    return;
};

players::players(std::string input, bool type): input(input), type(type){
    return;
};

// MAIN PLAYER CLASS

main_player::main_player(): players("", true){

    std::jthread log_thread(keylog<main_player>, this);
    
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
    log_thread.join();
    move::down(3);
}



void sleep(const double& sec){    
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));    
}
