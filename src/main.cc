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


int main(int argc, char* argv[]){

    /*
    std::cout << argc << NEW;
    for(int i =0; i<argc; i++){
        std::cout << argv[i] << NEW;
    }
    
    std::cin.get();
    */
    #if defined(NOSYNC)
        std::cout.sync_with_stdio(false);
    #endif
    
    move::clear();
    move::up_clear(std::stoi(argv[1])); // clears the loading message

    // THESE CHANGE
    // are mutable
    using namespace base64;
    std::string ACCESS_TOKEN = decode(argv[2]);
    std::string REFRESH_TOKEN = decode(argv[3]);
    int EXPIRES_AT = std::stoi(decode(argv[4]));
    

    main_player player(ACCESS_TOKEN, REFRESH_TOKEN, EXPIRES_AT);

    // PRINTS THE ARGUMENTS

    
    return 0;
}
