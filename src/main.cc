#include "header.h"


/*
FSPOT C++ CONVERSION

COMPILE MANUALLY:
python -m PyInstaller src/py/fmain.py --onefile --name fstart && cp dist/fstart fspot/
git-all && clang++ -lcpr -std=c++20 src/main.cc src/utils.cc -o fspot/fplayer 

everything:
clang++ -lcpr -std=c++20 src/main.cc src/utils.cc -o fspot/fplayer & python -m PyInstaller src/py/fmain.py --onefile --name fstart && cp dist/fstart fspot/



git-all && clang++ -Bdynamic -lcpr -Bstatic -l:libstdc++.a -l:libc.a -std=c++20 src/main.cc src/utils.cc -o fspot/fplayer -v

NOTES:
librespot is compiled with alsa backend
every response json variable must be stored in some temp variable


DO NOT USE EXIT(1), MEMORY LEAK WITH SMART POINTERS
*/


// CLOSING FUNCTIONS

static std::array<std::string, 5> argv;



int main(void){

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

    std::ifstream file(FOLDER+ ".tmp.txt");
    if (file.is_open())
    {
    	std::string tmp;
        int i =0;
    	while (getline(file, tmp))
        {
            argv[i] = tmp;
            i++;
        	// note that the newline character is not included
            // in the getline() function
        	
        }
    }
    file.close();
    system(("rm " + FOLDER + ".tmp.txt &> /dev/null").c_str());


    move::clear();
    move::up_clear(std::stoi(argv[0])); // clears the loading message

    // THESE CHANGE
    // are mutable


    static std::string ACCESS_TOKEN = base64::decode(argv[1]);
    static std::string REFRESH_TOKEN = base64::decode(argv[2]);
    static unsigned long EXPIRES_AT = std::stoi(base64::decode(argv[3]));

    // kills librespot pid

    //std::cout <<  base64::decode(argv[5]) << NEW << NEW << NEW;  //reassigns the ptr 
    //std::cout << argv[5];   

    // runs when program exits
    std::atexit([]{

        system(("rm " + FOLDER + ".cover.jpg &> /dev/null").c_str());
        system((std::string("kill -9 ")+base64::decode(argv[4])).c_str());
        print_logo();
        std::cout << TAB << INVERT_ON << "[ see ya... vro ]" << INVERT_OFF << NEW;
    });


    main_player player(ACCESS_TOKEN, REFRESH_TOKEN, EXPIRES_AT);
    

    // PRINTS THE ARGUMENTS

    
    return 0;
}
