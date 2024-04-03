#include <iostream>
#include <curl/curl.h>


int main(){
    CURL *curl = curl_easy_init();
    if(!curl) {
        std::cout << "ERROR";
        return EXIT_FAILURE;
    }

    curl_easy_cleanup(curl);

}
