#include "header.h"


// CONSTRUCTORS

// input and type initializer
players::players(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT, const int row_size): 
input(""), 
message(""),
type(true),
col_size(0),
row_size(row_size),
ACCESS_TOKEN(ACCESS_TOKEN),
REFRESH_TOKEN(REFRESH_TOKEN),
REFRESH_AT(REFRESH_AT),
log_thread(std::make_unique<std::jthread>(&players::keylog, this)),
col_thread(std::make_unique<std::jthread>(&players::col_update, this))
{
    log_thread->detach();
    col_thread->detach();
    
};

// CLASS DESTRUCTORS
players::~players(){};

main_player::~main_player(){

    move::clear();
    move::up_clear(row_size-2);

}

void players::MESSAGE(const std::string msg, const double time){
    std::jthread(&players::message_log, this, msg, time).detach();
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


// refreshes the token 
void players::refresh(){
    // refresh post request response
    MESSAGE("Refreshing...");
    cpr::Response r = cpr::Post(cpr::Url{TOKEN_URL},
                                cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Authorization", "Basic " + base64::encode(CLIENT_ID + ':' + CLIENT_SECRET)}
                                }, 
                                cpr::Parameters{
                                {"grant_type", "refresh_token"},
                                {"refresh_token", REFRESH_TOKEN}
                                });

    //if successful
    if (r.status_code == 200) { // if token was refreshed successfully
        //std::cout << r.text << NEW << NEW << NEW << NEW;
        MESSAGE("Token refreshed!", 1.0); // message
        json new_token = json::parse(r.text); // new token
        ACCESS_TOKEN = new_token["access_token"];
        REFRESH_AT = new_token["expires_in"];
        REFRESH_AT += POSIX_TIME;
    } else {
        MESSAGE("Token refresh failed");
    }
}

void players::col_update(){
    struct winsize w;
    while(1){
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        col_size = w.ws_col;
        SLEEP(0.01);
    }
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
                    playing = (bool)r["is_playing"];
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

// PLAYERS DEFAULTS

// default commands
void players::commands(){

    // guranteed all commands will contain different first and last chars
    json r; // temp response variable
    if (input == "quit"){ //quit
        MESSAGE("Quitting...");
        type = false;
        if (log_thread) log_thread->request_stop();

    } else if (input == "refresh") {
        std::jthread(&players::refresh, this).detach();

    } else if (input == "clear"){
        system("clear");

    } else if (input == "hello") {
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
            playing = (bool)r["is_playing"];
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

std::string players::CENTER(const std::string& input){
    if (input.empty()) {
        return std::string(""); 

    } else if (input.size() >= col_size) {
        move::up_clear(input, col_size);
        return "  " + input.substr(0,col_size-7) + "...  ";
    };

    int padding = (col_size-input.size())/2;
    return std::string((padding > 0) ? padding : 1, ' ') + input ;
};






// MAIN PLAYER CLASS


// main player constructor
main_player::main_player(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT): 
players(ACCESS_TOKEN, REFRESH_TOKEN, REFRESH_AT, 4),
song_thread(std::make_unique<std::jthread>(&main_player::song_update, this)), //updates every second
artist_thread(std::make_unique<std::jthread>(&main_player::artist_update, this)), //updates every second
progress(0), //progress is 0
duration(100), //duration is 100 to avoid division errors
artist_print(0), //prints no one duh
artists({"no one yet"}), //default json array
name("connecting ? maybe")
 {
    song_thread->detach();
    artist_thread->detach();
    
    std::jthread(&players::refresh, this).detach(); // refreshes the token
    //std::jthread log_thread(&main_player::keylog, this); //keylogging enabled
    
    move::down(row_size);
    move::up(row_size);

    while(type){ //keeps updating

        // prints minutes / seconds  of progress (in sec)
        std::string title = name + ((artists.size() >1) ? " : [" + std::to_string(artist_print+1) + "] " : " : ") + artists[artist_print];
        

        std::cout << CENTER(title) <<  NEW;
        printf("%02i:%02i\n\n", progress / 60, progress % 60);

        std::cout << CENTER(message) << '\r';
        std::cout<< INVERT_ON << " // " << input << INVERT_OFF; 
        //move::right(3+input.length());
        SLEEP(0.03);
        move::clear();

        std::cout << CENTER(message) << '\r';
        std::cout<< INVERT_ON << " // " << input << INVERT_OFF; 
        //move::right(3+input.length());
        //move::beginning();

        move::up_clear(row_size-1);

    }  
    move::down();

}



void main_player::song_update() {
    while(type){    
        cpr::Response r = cpr::Get(INTO("me/player"));
        if(r.status_code == 200){
            json data = json::parse(r.text);
            progress = (int)data["progress_ms"]; //progress in seconds
            progress /=1000; 

            
            auto item = data["item"];


            int tmp_dur = item["duration_ms"];
            tmp_dur /= 1000;
            std::string tmp_name = item["name"];

            // IF THERES BEEN A SONG SWITCH
            if(tmp_dur != duration && tmp_name != name){
                duration = tmp_dur;
                name = tmp_name;
                artists = {};
                for (const auto& artist: item["artists"]){
                    artists.push_back(artist["name"].get<std::string>());
                }
            }
        } else {
            progress = 0;
            duration = 100;
            artists = {"mr. nuh uh"};
            name = "NO CONNECTION";
        }   
        
        SLEEP(1);
    }
};

void main_player::artist_update() {
    while(1){
        if (artists.size()) artist_print = (artist_print+1) % artists.size();
        SLEEP(2);
    }
};


// HELPER FUNCTIONS

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
    void up_clear(const std::string& word_above, const int col_size) {
        up_clear(word_above.size()/col_size + 1);
    };

    void left()              { printf("\x1b[1D"); };
    void left(const int amt) { printf("\x1b[%iD", amt); };

    void right()              { printf("\x1b[1C"); };
    void right(const int amt) { printf("\x1b[%iC", amt); };


}
namespace base64{

    static std::string encode(const std::string &in) {

        std::string out;

        int val = 0, valb = -6;
        for (unsigned char c : in) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
                valb -= 6;
            }
        }
        if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
        while (out.size()%4) out.push_back('=');
        return out;
    }

    static std::string decode(const std::string &in) {

        std::string out;

        std::vector<int> T(256,-1);
        for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

        int val=0, valb=-8;
        for (unsigned char c : in) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(char((val>>valb)&0xFF));
                valb -= 8;
            }
        }
        return out;
    }

}