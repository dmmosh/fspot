#include <cpr/cpr.h>
#include <iostream>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{std::string("https://api.spotify.com/v1/" ) + "me/player"},
                                    cpr::Header{{"Authorization", "Bearer " + std::string("")}});
    std::cout << r.status_code;
}
