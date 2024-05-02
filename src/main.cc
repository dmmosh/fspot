#include "header.h"

/*
FSPOT C++ CONVERSION

COMPILE MANUALLY:
git-all && clang++ -std=c++20 src/main.cc src/utils.cc -o build/fspot && ./build/fspot/
*/

int main(void){
    //std::jthread main_thread(main_input, std::ref(input));
    std::thread main_thread(main_input);
    

    main_thread.join();
    return 0;
}