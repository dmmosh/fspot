#include "header.h"

/*
FSPOT C++ CONVERSION

COMPILE MANUALLY:
python -m PyInstaller src/py/fmain.py --onefile --name fstart && cp dist/fstart fspot/
git-all && clang++ -std=c++20 src/main.cc src/utils.cc -o fspot/fplayer && ./fspot/fplayer
*/

int main(int argc, char* argv[]){
    if (argc !=5) ERROR("Not enough arguments. I see what you did there.");
    

    move::clear();
    move::up_clear(std::stoi(argv[4])); // clears the loading message


    main_player player;
    

    // PRINTS THE ARGUMENTS
    /*
    for(int i =0; i<argc; i++){
        std::cout << argv[i] << NEW;
    }
    */
    std::cin.get();
    
    return 0;
}