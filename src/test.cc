#include <vector>
#include <memory>
#include <chrono>
#include <cstdio>
#include <string>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>
#include <cstring>
#include <cpr/cpr.h>
#include <bits/stdc++.h> 
#include <sys/ioctl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

std::string exec(const std::string& cmd) {
    std::array<char, 200> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) { //exception handle
        throw std::runtime_error("popen() failed!");
    } 

    std::string output = "";

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {

        output.append(buffer.data());
    }
    return output;
}

int main() {
    // Specify image file path
    std::string url = "https://i.scdn.co/image/ab67616d00001e02ff9ca10b55ce82ae553c8228";
    
    // Perform the HTTP GET request
    auto response = cpr::Get(cpr::Url{url});
    // Check if the request was successful
    if (response.status_code == 200) {
        // Open a file stream to save the downloaded image
        std::ofstream imageFile(".image.jpg", std::ofstream::binary);
        
        // Write the image data to the file
        imageFile.write(response.text.c_str(), response.text.length());
        
        // Close the file stream
        imageFile.close();
    }

    std::cout << exec("icat  .image.jpg");
    system("rm .image.jpg");

    return 0;
}