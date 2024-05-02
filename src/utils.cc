#include "header.h"


namespace move {
void line()                           { std::cout << "\x1b[2K\r"; }
void line(const std::string& newline) { std::cout << "\x1b[2K\r" <<  newline;  };

void up()        { std::cout << "\x1b[1A"; };
void up(int amt) { while(amt) { move::up(); amt--; } };

void clear()        { move::up(); move::clear(); }
void clear(int amt) { while(amt) { move::clear(); amt--; } }

}

void sleep(const double& sec){    
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));    
}


void main_input(){
    std::string input = "fsdlkjl";
    
    while(1){
        std::cout << input << N << N;

        std::cout<< "\n// " << input; 
        sleep(0.2);
        move::clear();
        std::cout<< "// " << input; 

        move::up();
        move::clear(2);
    }  
}



