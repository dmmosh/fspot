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

#if defined(HAVE_SYS_SELECT_H)
#include <sys/select.h>
#endif
#undef glob
extern "C" int glob_old(const char * pattern, int flags, int (*errfunc) (const char *epath, int eerrno), glob_t *pglob);
#ifdef __i386__
    __asm__(".symver glob_old,glob@GLIBC_2.1");
#elif defined(__amd64__)
    __asm__(".symver glob_old,glob@GLIBC_2.2.5");
#elif defined(__arm__)
    __asm(".symver glob_old,glob@GLIBC_2.4");
#elif defined(__aarch64__)
    __asm__(".symver glob_old,glob@GLIBC_2.17");
#endif

extern "C" int __wrap_glob(const char * pattern, int flags, int (*errfunc) (const char *epath, int eerrno), glob_t *pglob)
{
    return glob_old(pattern, flags, errfunc, pglob);
}

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
    std::string ACCESS_TOKEN = argv[2];
    std::string REFRESH_TOKEN = argv[3];
    int EXPIRES_AT = std::stoi(argv[4]);
    

    main_player player(ACCESS_TOKEN, REFRESH_TOKEN, EXPIRES_AT);

    // PRINTS THE ARGUMENTS

    
    return 0;
}
