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





// PLAYERS DEFAULTS

// default commands
void players::commands(){
    if (input == "quit"){ //quit
        type = false;
    } else if (input == "play"){
        cpr::Response r = cpr::Put(cpr::Url{BASE_URL + "me/player/play"},
                                    cpr::Header{{"Authorization", "Bearer " + std::string("BQC0pzTd5lO7S7-_P5gnL88cmoIMg-FcJ87m3pCGCoTJia84O5iPW1eGgI3LMEbvcK0kv7ujcmHiFNre7om25DKV-lbw_lLCeRPays_3sWYVV7fZ6Nn9piC78PFulOIOJnLkfGiz4rWDz8Gi5ZsH6kZYkxiMfjb_dO4yOXbOBUUd2V8h2e-wlY5Ds2PEmfYfEIvteKWo7gYXeHwF2WJNZpAs0MrOpquwwSII4rzEQhDhbAKQwkIfvbnR9waeaHFmceGt4hKcHg")}});
    }
    
    return;
};




// input and type initializer
players::players(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT): 
input(""), 
type(true),
ACCESS_TOKEN(ACCESS_TOKEN),
REFRESH_TOKEN(REFRESH_TOKEN),
REFRESH_AT(REFRESH_AT),
log_thread(std::make_unique<std::jthread>(&players::keylog, this))
{};


players::~players(){
    if (log_thread) log_thread->join();
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
main_player::main_player(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT): 
players(ACCESS_TOKEN, REFRESH_TOKEN, REFRESH_AT) {

    //std::jthread log_thread(&main_player::keylog, this); //keylogging enabled
    
    move::down(3);
    move::up(3);
    while(type){ //keeps updating

        
        std::cout << input << NEW << NEW;

        std::cout<< INVERT_ON << " // " << input << INVERT_OFF ; 
        //move::right(3+input.length());
        sleep(0.01);
        move::clear();
        std::cout<< INVERT_ON << " // " << input << INVERT_OFF; 
        //move::right(3+input.length());
        //move::beginning();

        move::up_clear(2);

    }  
    move::down();
}

main_player::~main_player(){
    move::clear();
    move::up_clear(2);
}