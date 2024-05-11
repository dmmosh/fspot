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


// CLOSING FUNCTIONS


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
    static std::string ACCESS_TOKEN = base64::decode(argv[2]);
    static std::string REFRESH_TOKEN = base64::decode(argv[3]);
    static int EXPIRES_AT = std::stoi(base64::decode(argv[4]));

    // kills librespot pid
    static char* pid_encode = argv[5];

    //std::cout <<  base64::decode(argv[5]) << NEW << NEW << NEW;  //reassigns the ptr 
    //std::cout << argv[5];   

    // runs when program exits
    std::atexit([]{
        system((std::string("kill -9 ")+base64::decode(pid_encode)).c_str());
    });


    main_player player(ACCESS_TOKEN, REFRESH_TOKEN, EXPIRES_AT);
    
    print_logo();
    std::cout << NEW << TAB << INVERT_ON << "[ see ya... vro ]" << INVERT_OFF << NEW;

    // PRINTS THE ARGUMENTS

    
    return 0;
}
