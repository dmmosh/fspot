#include "header.h"


// CONSTRUCTORS

// input and type initializer
players::players(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT, const int row_size): 
input(""), 
message(""),
type(true),
progress(0), //progress is 0
duration(100), //duration is 100 to avoid division errors
percent(0.0),
is_playing(false),
artist_print(0), //prints no one duh
artists({"no one yet"}), //default json array
name("connecting ? maybe"),
row_size(row_size),
ACCESS_TOKEN(ACCESS_TOKEN),
REFRESH_TOKEN(REFRESH_TOKEN),
REFRESH_AT(REFRESH_AT),
log_thread(std::jthread(&players::keylog, this))
{
    log_thread.detach();
    
};

// CLASS DESTRUCTORS
players::~players(){};

main_player::~main_player(){

    move::clear();
    move::up_clear(row_size-2);

}

// MESSAGES 
// mini message: meant to be turned off, doesnt use thread
// message: automatically turns off after a given time USE ONLY WHEN NEEDED

// MINI MESSAGE : when it changes a lot
// meant to be turned off, will stay forever if doesnt 
void players::MINI_MESSAGE(const std::string& msg){
    message = (std::string(INVERT_ON) + "[ " + msg + " ]" + INVERT_OFF ); //assigns new message
}

// standard message 
void players::MESSAGE(const std::string msg, const double time){
    MINI_MESSAGE(msg);
    std::jthread(&players::message_log, this, time).detach();
}

void players::MESSAGE(const std::string msg){
    MESSAGE(msg, 5.0);
};

void players::MESSAGE(){
    MESSAGE("Loading...", 5.0);
}


void players::message_log(const double time){
    std::string temp = message; // temp string
    SLEEP(time); // waits the time
    if (temp == message) MESSAGE_OFF; // turn message off only if theres no new message to replace it
}


// refreshes the token 
int players::refresh(){
    // refresh post request response
    MINI_MESSAGE("Refreshing...");
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
    return r.status_code;
}


// CHARACTER INPUT  and keylog
// any subclass
void players::keylog(){
        int ff_sec_prev = 0;
        int ff_sec = 1;
        int x = 0;
        int max = 1;

        
        while(type){
        
        char buf = get_char();

        switch(buf){
            case 0: // if theres nothing in the buffer
                continue;
            break;
            case ENTER:
                commands();
                input = "";
            break;  
            case SPACE:


                if (is_playing){
                    MINI_MESSAGE("Pausing...");
                    (void)cpr::Put(INTO("me/player/pause"));
                    MESSAGE("Paused!", 1);
                    is_playing= false;
                } else {
                    MINI_MESSAGE("Playing...");
                    (void)cpr::Put(INTO("me/player/play"));
                    MESSAGE("Playing now!", 1);
                    is_playing= true;
                };
            break;
            case BACKSPACE:
                if (input.size()) input.resize(input.size() - 1);
            break;
            case '.': //forward 10 seconds
                //std::jthread(&players::forward, this, std::ref(ff_sec_prev), std::ref(ff_sec), std::ref(x), std::ref(max), true).detach();
                //forward(ff_sec_prev, ff_sec, x, max, true);
                
                do{

                    x++;
                    ff_sec = forward_fun(x);
                    MINI_MESSAGE("+" + std::to_string(ff_sec)); 
                    
                    SLEEP(0.1);
                    //MESSAGE_OFF;
                } while (get_char());

                MESSAGE("+" + std::to_string(ff_sec) + "sec...", 1.0);
                x =0;
                ff_sec= 1;

                
            break;
            case ',':
                std::jthread(&players::forward, this, std::ref(ff_sec_prev), std::ref(ff_sec), std::ref(x), std::ref(max), false).detach();
            break;
            case '>':
                MINI_MESSAGE("Nexting...");


                (void)cpr::Post(INTO("me/player/next"));
                SLEEP(0.5); //need a cooldown
            break;
            case '<':
                MINI_MESSAGE("Previousing...");
                (void)cpr::Post(INTO("me/player/previous"));
                SLEEP(0.5); //need a cooldown
            break;
            default:
                if (input.length() <15)
                    input.push_back(tolower(buf));
        }     
        SLEEP(0.0001);
    }
}

// PLAYERS DEFAULTS

void players::forward(int& ff_sec_prev, int& ff_sec, int& x, int& max, const bool forward_back){

    x++;
    ff_sec= forward_fun(x);
    //if (x < 5) ff_sec++;   

    if (ff_sec > max) max = ff_sec;


    MESSAGE( ((forward_back) ? "+" : "-") + std::to_string(max), 1); 

    //SLEEP(1);
    if (ff_sec_prev == ff_sec && max>1){ // when user releases 
        
        switch((int)forward_back){
            case 1: // TO GO FORWARD
            if (max >= 3600){
                MESSAGE("nice try buddy", 3.0);

            } else if (progress+max > duration){
                MESSAGE("Nexting...");
                (void)cpr::Post(INTO("me/player/next"));

            } else {   // if it doesnt, actually go forward
                MESSAGE( "+" + std::to_string(max) + " sec...");

                (void)cpr::Put(INTO("me/player/seek"),
                            cpr::Parameters{{"position_ms", std::to_string((progress + max)*1000)}});
            }
            
            break;
            default: // TO GO BACKWARD
            

            if (max >= 3600){
                MESSAGE("nice try buddy");
            
            } else if (progress-max <= 0){
                MESSAGE("wind it back!");
                (void)cpr::Put(INTO("me/player/seek"),
                            cpr::Parameters{{"position_ms", "0"}});

            } else {   // if it doesnt, actually go forward
                MESSAGE( "-" + std::to_string(max) + " sec...");

                (void)cpr::Put(INTO("me/player/seek"),
                            cpr::Parameters{{"position_ms", std::to_string((progress - max)*1000)}});
            }


        }

        MESSAGE_OFF;
        ff_sec_prev = 0;
        ff_sec = 1;
        x = 0;
        max = 1;
        return;
    }   


    std::jthread([&ff_sec, &ff_sec_prev]() {
                    SLEEP(1);
                    ff_sec_prev = ff_sec;
    });
    
}

// default commands
void players::commands(){

    // guranteed all commands will contain different first and last chars
    json r; // temp response variable
    if (input == "quit"){ //quit
        MINI_MESSAGE("Quitting...");
        type = false;
        log_thread.request_stop();

    } else if (input == "refresh") {
        refresh();

    } else if (input == "clear"){
        system("clear");

    } else if (input == "hello") {
        MESSAGE("Hello vro...", 2.0);
    } else if (input == "play"){ // plays track
        MINI_MESSAGE("Playing...");
        (void)cpr::Put(INTO("me/player/play"));
        MESSAGE("Playing now!", 1);
        is_playing= true;


    } else if (input == "pause"){ // pauses track
        MINI_MESSAGE("Pausing...");
        (void)cpr::Put(INTO("me/player/pause"));
        MESSAGE("Paused!", 1);
        is_playing= false;


    } else if (input == "pp") { //plays / pauses track

        if (is_playing){
            MINI_MESSAGE("Pausing...");
            (void)cpr::Put(INTO("me/player/pause"));
            MESSAGE("Paused!", 1);
            is_playing= false;

        } else {
            MINI_MESSAGE("Playing...");
            (void)cpr::Put(INTO("me/player/play"));
            MESSAGE("Playing now!", 1);
            is_playing= true;

        };

    }
    return;
};

// NOTE: changes the input variable if needs concat
std::string players::CENTER(std::string input){
    unsigned int col_size = col_update();
    if (input.empty()) {
        input = std::string("");

    } else if (input.size()+4 >= col_size) {
        input = CENTER(input.substr(0,col_size-8) + "...");
    } else {
        int padding = (col_size-input.size())/2;
        input.insert(0, std::string((padding > 0) ? padding : 1, ' '));
    };

    return input;
};






// MAIN PLAYER CLASS


// main player constructor
main_player::main_player(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT): 
players(ACCESS_TOKEN, REFRESH_TOKEN, REFRESH_AT, 4),
song_thread(std::jthread(&main_player::song_update, this)), //updates every second
artist_thread(std::jthread(&main_player::artist_update, this)) //updates every second
 {
    song_thread.detach();
    artist_thread.detach();
    
    std::jthread(&players::refresh, this).detach(); // refreshes the token
    //std::jthread log_thread(&main_player::keylog, this); //keylogging enabled
    
    move::down(row_size);
    move::up(row_size);

    while(type){ //keeps updating
        unsigned int col_size = col_update();
        // prints minutes / seconds  of progress (in sec)
        std::string title = name + ((artists.size() >1) ? " : [" + std::to_string(artist_print+1) + "] " : " : ") + artists[artist_print];
        
        std::cout << CENTER(title) <<  NEW;

        if (col_size > 10) {
            int bar_size = (col_size-20 > 0) ? col_size-20 : col_size-10;

            std::string bar = (((int)(bar_size*percent) > 0) ? std::string((int)(bar_size*percent), '-') : "") + 
                              (((int)(bar_size*(1.0-percent)) > 0) ? std::string((int)(bar_size*(1.0-percent)), ' ') : "") ;
            std::cout << BOLD_ON << CENTER("<" + bar + ">")  << BOLD_OFF << '\r';
        }

        printf("%02i:%02i\n\n", progress / 60, progress % 60);

        std::cout<< INVERT_ON << " // " << input << TAB << message <<  INVERT_OFF; 
        //move::right(3+input.length());
        SLEEP(0.03);
        move::clear();

        std::cout<< INVERT_ON << " // " << input << TAB << message << INVERT_OFF; 
        //move::right(3+input.length());
        //move::beginning();

        move::up_clear(row_size-1);

    }  
    move::down();

}



void main_player::song_update() {
    static int tmp_dur = 0;
    static std::string tmp_name = "NO CONNECTION";
    while(type){    
        cpr::Response r = cpr::Get(INTO("me/player"));
        if(r.status_code == 200){
            json data = json::parse(r.text);
            is_playing = (bool)data["is_playing"];
            auto item = data["item"];

            


            progress = (int)data["progress_ms"]; //progress in seconds
            progress /=1000; 


            duration = (int)item["duration_ms"];
            duration/=1000;

            percent = (double)progress/duration;

            name = item["name"];

            // IF THERES BEEN A SONG SWITCH
            if(tmp_dur != duration && tmp_name != name){
                MESSAGE("New track!", 1.0);

                if (tmp_dur >= 3600) {
                    type = false;    
                    ERROR("Song's too long. FSpot can't play songs longer than 1 hour.");
                };

                if (POSIX_TIME + (tmp_dur-progress) >= REFRESH_AT) refresh(); //if next song is over the token expire, refresh it
                
                artists = {};
                for (const auto& artist: item["artists"]){
                    artists.push_back(artist["name"].get<std::string>());
                }
            }


            tmp_dur = duration;
            tmp_dur /= 1000;

            tmp_name = name;

        } else {
            progress = 0;
            percent = 0;
            duration = 100;
            artists = {"mr. nuh uh"};
            name = "NO CONNECTION";
            refresh();
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

unsigned int col_update(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

constexpr int forward_fun(const int x_val){
    return (int)((double)x_val*x_val/50);
};

void print_logo(){
    unsigned int col_size = col_update();

    unsigned int title_num;


    if (col_size < 42)
        title_num = 1;
    else if (col_size < 52)
        title_num = 2;
    else if (col_size < 123)
        title_num = 3;
    else 
        title_num = 4;
    
    std::ifstream title_text(FOLDER + "titles/title" + std::to_string(title_num) + ".txt");

    if (title_text.is_open()) {
        std::string line;
        while (std::getline(title_text, line)) {
            std::cout << BOLD_ON << line << BOLD_OFF << NEW; // Print each line
        }
        title_text.close(); // Close the file
    } else {
        std::cout << BOLD_ON << "FSPOT" << BOLD_OFF << NEW;
    }
    move::up();
};

char get_char(){
        
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 0;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    
    return buf;

};


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
        if(word_above.size() < col_size) {
            up_clear();
        } else {
            up_clear((int)ceil((double)word_above.size()/(double)col_size));
        }
    };

    void left()              { printf("\x1b[1D"); };
    void left(const int amt) { printf("\x1b[%iD", amt); };

    void right()              { printf("\x1b[1C"); };
    void right(const int amt) { printf("\x1b[%iC", amt); };


}
namespace base64{


    std::string encode(const std::string &in) {

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

    std::string encode(const char* in){
        return encode(std::string(in));
    };

    std::string decode(const std::string &in) {

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

    std::string decode(const char* in) {
        return decode(std::string(in));
    };

}