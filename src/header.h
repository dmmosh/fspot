#include <vector>
#include <memory>
#include <chrono>
#include <cstdio>
#include <string>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>
#include <cstring>
#include <cpr/cpr.h>
#include <bits/stdc++.h> 
#include <sys/ioctl.h>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

// MACROS

// special characters
#define NEW '\n'
#define TAB '\t'
#define ENTER 10
#define SPACE 32
#define BACKSPACE 127

#define INTO(x) cpr::Url{BASE_URL + x}, cpr::Header{{"Authorization", "Bearer " + ACCESS_TOKEN}}


#define GET_JSON(params) json::parse(cpr::Get(params).text)  // returns get request output in json format
#define POST_JSON(params) json::parse(cpr::Post(params).text)  // returns get request output in json format
#define PUT_JSON(params) json::parse(cpr::Put(params).text)  // returns get request output in json format
#define DELETE_JSON(params) json::parse(cpr::Delete(params).text)  // returns get request output in json format



#define AUTH_URL std::string("https://accounts.spotify.com/authorize") 
#define TOKEN_URL std::string("https://accounts.spotify.com/api/token" )
#define BASE_URL std::string("https://api.spotify.com/v1/" )
#define CLIENT_ID std::string("bbdff8f6b6524edc90d968c3f971b5da")
#define CLIENT_SECRET std::string(getenv("FSPOT_CLIENT_SECRET"))
#define FOLDER (std::string(getenv("HOME")) + "/coding/fspot/fspot/")

#define BOLD_ON "\033[1m"
#define BOLD_OFF "\033[0m"
#define INVERT_ON "\033[;7;1m"
#define INVERT_OFF "\033[0m"
#define DEFAULT_ON "\033[39;49m"
#define DEFAULT_OFF "\033[0m"
#define LINE_ON "\x1b[41;37;1m\033[;7;1m"
#define LINE_OFF "\x1b[K\x1b[0m"


// fatal error message
#define POSIX_TIME (long)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define ERROR(msg) std::cout << NEW << LINE_ON << "FATAL ERROR:" << LINE_OFF << NEW <<  LINE_ON << "   " << msg << LINE_OFF << NEW << NEW << NEW << NEW << NEW << NEW
#define SLEEP(sec) std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)))
#define MESSAGE_OFF message = "" // turns the message off



// PLAYER BASE CLASS
class players{
    public:

    // VARIABLES

    // always updating
    std::string input, message;

    std::atomic<int> progress, duration, artist_print;
    std::atomic<bool> is_playing, cover;
    std::atomic<double> percent;
    std::vector<std::string> artists;
    std::string name;
    
    // sometimes updating
    std::atomic<bool> type;

    std::string ACCESS_TOKEN, REFRESH_TOKEN; //access and refrehs wokens
    unsigned long REFRESH_AT; // posix timestamp of when to refresh (seconds since 1970)
    
    // never updating
    std::atomic<int> row_size;
    std::jthread log_thread;
    


    players(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, unsigned long& REFRESH_AT, const int row_size);
    ~players();
    void keylog();
    void commands();
    int refresh();
    
    void forward(const bool forward_back); //forward-backward function
    void volume(const bool add_substr);
    void cover_fun(const std::string& url);

    void MINI_MESSAGE(const std::string& msg);
    void MINI_MESSAGE(const char* msg);
    void MESSAGE(const std::string msg, const double time);
    void MESSAGE(const std::string msg);
    void MESSAGE();
    void message_log(const double time);

    // MESSAGE 

};

// MAIN PLAYER SUBCLASS
class main_player: public players{
    public:
    // VARIABLES

    // never updating
    std::jthread song_thread;

    main_player(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, unsigned long& REFRESH_AT);
    ~main_player();
    
    void commands();
    void song_update(); //updates song info EVERY SECOND
};


// HELPER FUNCTIONS


unsigned int col_update();
unsigned int row_update();
void print_logo();
char get_char();
static char* timer(const int seconds);

std::string CENTER( std::string input);
std::string CENTER( std::string input, const unsigned int col_size);

std::string exec(const std::string& cmd);


constexpr int forward_fun(const int x_val);

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
     void up_clear(const std::string& word_above, const int col_size);
    

     void left();
     void left(const int amt);

     void right();
     void right(const int amt);
};


namespace base64{
    std::string encode(const std::string &in);
    std::string encode(const char* in);

    std::string decode(const std::string &in);
    std::string decode(const char* in);

};