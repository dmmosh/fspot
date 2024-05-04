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

void players::MESSAGE(const std::string msg, const double time){
    std::jthread test(&players::message_log, this, msg, time);
    test.detach();
}

void players::MESSAGE(const std::string msg){
    MESSAGE(msg, 5.0);
};

void players::MESSAGE(){
    MESSAGE("Loading...", 5.0);
}

void players::message_log(const std::string msg, const double time){
    message = (std::string(INVERT_ON) + "[ " + msg + " ]" + INVERT_OFF ); //assigns new message
    std::string temp = message; // temp string
    SLEEP(time); // waits the time
    if (temp == message) MESSAGE_OFF; // turn message off only if theres no new message to replace it
}




// PLAYERS DEFAULTS

// default commands
void players::commands(){

    // guranteed all commands will contain different first and last chars
    json r; // temp response variable
    if (input == "quit"){ //quit
        MESSAGE("Quitting...");
        type = false;
    } else if (input == "hello"){
        MESSAGE("Hello vro...", 2.0);
    } else if (input == "play"){ // plays track
        MESSAGE("Playing...");
        (void)cpr::Put(INTO("me/player/play"));
        MESSAGE_OFF;
        MESSAGE("Playing now!", 1);

    } else if (input == "pause"){ // pauses track
        MESSAGE("Pausing...");
        (void)cpr::Put(INTO("me/player/pause"));
        MESSAGE_OFF;
        MESSAGE("Paused!", 1);

    } else if (input == "pp") { //plays / pauses track
        bool playing = false;
        try{
            r = GET_JSON(INTO("me/player"));
            playing = r["is_playing"];
        }
        catch(...) {};

        if (playing){
            MESSAGE("Pausing...");
            (void)cpr::Put(INTO("me/player/pause"));
            MESSAGE_OFF;
            MESSAGE("Paused!", 1);
        } else {
            MESSAGE("Playing...");
            (void)cpr::Put(INTO("me/player/play"));
            MESSAGE_OFF;
            MESSAGE("Playing now!", 1);
        };

    }
    return;
};



// input and type initializer
players::players(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT, const int row_size): 
input(""), 
message(""),
type(true),
row_size(row_size),
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

        json r;
        bool playing = false;
        switch(buf){
            case ENTER:
                commands();
                input = "";
            break;  
            case SPACE:
                try{
                    r = GET_JSON(INTO("me/player"));
                    playing = r["is_playing"];
                }
                catch(...) {};

                if (playing){
                    MESSAGE("Pausing...");
                    (void)cpr::Put(INTO("me/player/pause"));
                    MESSAGE_OFF;
                    MESSAGE("Paused!", 1);
                } else {
                    MESSAGE("Playing...");
                    (void)cpr::Put(INTO("me/player/play"));
                    MESSAGE_OFF;
                    MESSAGE("Playing now!", 1);
                };
            break;
            case BACKSPACE:
                if (input.size()) input.resize(input.size() - 1);
            break;
            default:
                if (input.length() <15)
                    input.push_back(tolower(buf));
        }
    }
}


// MAIN PLAYER CLASS

// main player constructor
main_player::main_player(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT): 
players(ACCESS_TOKEN, REFRESH_TOKEN, REFRESH_AT, 4) {
    //std::jthread log_thread(&main_player::keylog, this); //keylogging enabled
    
    move::down(row_size);
    move::up(row_size);

    int time = 0;
    while(type){ //keeps updating
        json status = GET_JSON(INTO("me/player"));
        time = (int)status["progress_ms"];

        std::cout << time << NEW;
        std::cout << input << NEW << NEW;

        std::cout<< INVERT_ON << " // " << input << INVERT_OFF << TAB << message; 
        //move::right(3+input.length());
        SLEEP(0.01);
        move::clear();
        std::cout<< INVERT_ON << " // " << input << INVERT_OFF << TAB << message; 
        //move::right(3+input.length());
        //move::beginning();

        move::up_clear(row_size-1);

    }  
    move::down();
}

main_player::~main_player(){
    move::clear();
    move::up_clear(row_size);
}

// HELPER FUNCTIONS

