#include "header.h"
#include <iostream>
#include <stdlib.h> 

#include <cpr/cpr.h> // You need to install cpr library. Use: `sudo apt-get install libcurl4-openssl-dev`
#include <nlohmann/json.hpp> // You need to install nlohmann/json library. Use: `sudo apt-get install nlohmann-json3-dev`

using json = nlohmann::json;

static std::string base64_encode(const std::string &in) {

    std::string out;

    int val = 0, valb = -6;
    for (char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

static std::string base64_decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

// Spotify API endpoints
const std::string SPOTIFY_API_URL = "https://accounts.spotify.com/api/token";

// Spotify API client credentials
const std::string CLIENT_ID = "bbdff8f6b6524edc90d968c3f971b5da";
const std::string CLIENT_SECRET = getenv("FSPOT_CLIENT_SECRET");

// Function to get Spotify client credentials
std::string getSpotifyClientCredentials() {
    // Set up request payload
    json payload = {
        {"grant_type", "client_credentials"}
    };

    // Set up authentication header
    std::string authHeader = CLIENT_ID + ":" + CLIENT_SECRET;
    authHeader = "Basic " + base64_encode(authHeader);
    // Set up HTTP headers
    cpr::Header headers = {
        {"Authorization", authHeader},
        {"Content-Type", "application/x-www-form-urlencoded"}
    };

    // Make POST request to Spotify API
    auto response = cpr::Post(cpr::Url{SPOTIFY_API_URL},
                              cpr::Body{payload.dump()},
                              cpr::Header{headers});

    // Check if request was successful
    if (response.status_code != 200) {
        std::cerr << "Error: Failed to get client credentials: " << response.text << std::endl;
        return "";
    }

    // Parse response JSON
    json jsonResponse = json::parse(response.text);
    std::string accessToken = jsonResponse["access_token"];

    return accessToken;
}

int main() {
    // Get client credentials from Spotify API
    std::string accessToken = getSpotifyClientCredentials();

    // Print access token
    std::cout << "Access Token: " << accessToken << std::endl;

    return 0;
}