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

#define SLEEP(sec) std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec*1000)));


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

int main(){
    char buf;
    while(1){
        buf = get_char();
        switch (buf){
            case 0:
            std::cout << "BROWN BABANA" << '\n';
            break;
            default:
            std::cout << buf << '\n';
        }
        SLEEP(0.2);

    }
}