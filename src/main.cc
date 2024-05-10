#include "header.h"


/*
FSPOT C++ CONVERSION

COMPILE MANUALLY:
python -m PyInstaller src/py/fmain.py --onefile --name fstart && cp dist/fstart fspot/
git-all && clang++ -lcpr -std=c++20 src/main.cc src/utils.cc -o fspot/fplayer 


git-all && clang++ -Bdynamic -lcpr -Bstatic -l:libstdc++.a -l:libc.a -std=c++20 src/main.cc src/utils.cc -o fspot/fplayer -v

NOTES:
librespot is compiled with alsa backend
every response json variable must be stored in some temp variable


DO NOT USE EXIT(1), MEMORY LEAK WITH SMART POINTERS
*/

static char* pid_encode= NULL; //pid encode ptr

void close(){
        std::string pid = base64::decode(pid_encode);
        system(("pkill -f "+pid+";sleep 2; pkill -9 -f "+pid).c_str());
};

int main(int argc, char* argv[]){

    /*
    std::cout << argc << NEW;
    for(int i =0; i<argc; i++){
        std::cout << argv[i] << NEW;
    }
    
    std::cin.get();
    */

    std::cout.sync_with_stdio(false);


    move::clear();
    move::up_clear(std::stoi(argv[1])); // clears the loading message

    // THESE CHANGE
    // are mutable
    static std::string ACCESS_TOKEN = base64::decode(argv[2]);
    static std::string REFRESH_TOKEN = base64::decode(argv[3]);
    static int EXPIRES_AT = std::stoi(base64::decode(argv[4]));

    pid_encode = argv[5];  //reassigns the ptr 
    

    std::atexit(close);

    main_player player(ACCESS_TOKEN, REFRESH_TOKEN, EXPIRES_AT);

    // PRINTS THE ARGUMENTS

    
    return 0;
}
