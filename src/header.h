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
#define ERROR(msg) std::cout << LINE_ON << "FATAL ERROR:" << LINE_OFF << NEW <<  LINE_ON << "   " << msg << LINE_OFF; exit(0)
#define SLEEP(sec) std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));
#define MESSAGE_OFF message = "" // turns the message off




// PLAYER BASE CLASS
class players{
    public:
    std::string input;
    std::string message;
    int row_size;
    bool type;
    std::unique_ptr<std::jthread> log_thread;
    std::string ACCESS_TOKEN, REFRESH_TOKEN;
    long REFRESH_AT; // posix timestamp of when to refresh (seconds since 1970)
    

    players(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT, const int row_size);
    ~players();
    void keylog();
    void commands();
    void refresh();

    void MESSAGE(const std::string msg, const double time);
    void MESSAGE(const std::string msg);
    void MESSAGE();
    void message_log(const std::string msg, const double time);

    // MESSAGE 

};

// MAIN PLAYER SUBCLASS
class main_player: public players{
    public:
    int progress, duration;
    std::string artist;
    std::string name;
    std::unique_ptr<std::jthread> song_thread;
    main_player(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT);
    ~main_player();

    void commands();
    void song_update(); //updates song info EVERY SECOND

};

// HELPER FUNCTIONS

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

namespace base64{
    static std::string encode(const std::string &in);
    static std::string decode(const std::string &in);

};




