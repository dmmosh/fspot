#include <cpr/cpr.h>
#include <iostream>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{std::string("https://api.spotify.com/v1/" ) + "me/player"},
                                    cpr::Header{{"Authorization", "Bearer " + std::string("BQC0pzTd5lO7S7-_P5gnL88cmoIMg-FcJ87m3pCGCoTJia84O5iPW1eGgI3LMEbvcK0kv7ujcmHiFNre7om25DKV-lbw_lLCeRPays_3sWYVV7fZ6Nn9piC78PFulOIOJnLkfGiz4rWDz8Gi5ZsH6kZYkxiMfjb_dO4yOXbOBUUd2V8h2e-wlY5Ds2PEmfYfEIvteKWo7gYXeHwF2WJNZpAs0MrOpquwwSII4rzEQhDhbAKQwkIfvbnR9waeaHFmceGt4hKcHg")}});
    std::cout << r.status_code;
}
