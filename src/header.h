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
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// MACROS

// special characters
#define NEW '\n'
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

#define BOLD_ON "\033[1m"
#define BOLD_OFF "\033[0m"
#define INVERT_ON "\033[;7;1m"
#define INVERT_OFF "\033[0m"
#define DEFAULT_ON "\033[39;49m"
#define DEFAULT_OFF "\033[0m"
#define LINE_ON "\x1b[41;37;1m\033[;7;1m"
#define LINE_OFF "\x1b[K\x1b[0m"

// fatal error message
#define ERROR(msg) std::cout << LINE_ON << "FATAL ERROR:" << LINE_OFF << NEW <<  LINE_ON << "   " << msg << LINE_OFF; exit(0)
#define SLEEP(sec) std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));

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





// PLAYER BASE CLASS
class players{
    public:
    std::string input;
    bool type;
    std::unique_ptr<std::jthread> log_thread;
    std::string ACCESS_TOKEN, REFRESH_TOKEN;
    int REFRESH_AT;
    

    players(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT);
    ~players();
    void keylog();
    void commands();


};

// MAIN PLAYER SUBCLASS
class main_player: public players{
    public:
    main_player(std::string& ACCESS_TOKEN, std::string& REFRESH_TOKEN, int& REFRESH_AT);
    ~main_player();

    void commands();

};


