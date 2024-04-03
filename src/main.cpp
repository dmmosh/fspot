// LIBRARIES
#include <fstream>
#include <iostream>
#include <string>
#include <boost/archive/text_oarchive.hpp> // saving
#include <boost/archive/text_iarchive.hpp> //loading


// DEFINITIONS
#define SAVE boost::archive::text_oarchive  //serialize
#define LOAD boost::archive::text_iarchive  //deserialize
#define OUT std::ofstream //output file stream
#define IN std::iftream //input file stream

// DEBUG DEFINITIONS

//TODO: change to `/usr/share/fspot`
#define F(x) std::string("./fspot/") + x // the helper file folder 

/*
FSPOT
a fast and lightweight cli client for spotify


important to link boost serialization library 
(see all libraries by running `ls /usr/lib/ | grep boost`)


MANUAL COMPILE:
g++ src/main.cpp -o ./exec -lboost_serialization;


COMPILE:
git-all; clang++ src/main.cpp -o ./exec -lboost_serialization; ./exec 
*/


class gps_position
{

    // required for class
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & degrees;
        ar & minutes;
        ar & seconds;
    }
public:
    int degrees;
    int minutes;
    float seconds;
    gps_position(){};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {}
};

int main() {
    // create and open a character archive for output
    OUT ofs(F("test"));

    // create class instance
    const gps_position g(35, 59, 24.567f);

    // save data to archive
    {
        SAVE oa(ofs);
        // write class instance to archive
        oa << g;
    	// archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its orginal state
    gps_position newg;
    {
        // create and open an archive for input
        std::ifstream ifs(F("test"));
        LOAD ia(ifs);
        // read class state from archive
        ia >> newg;
        std::cout << newg.degrees;
        // archive and stream closed when destructors are called
    }
    return 0;
}