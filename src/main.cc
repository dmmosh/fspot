#include "header.h"

/*
FSPOT C++ CONVERSION

COMPILE MANUALLY:
python -m PyInstaller src/fmain.py --onefile --name fstart && cp dist/fstart fspot/
git-all && clang++ -std=c++20 src/main.cc src/utils.cc -o dist/fspot && ./dist/fspot/
*/

int main(void){
    std::jthread main_thread(main_input);
    

    main_thread.join();
    return 0;
}