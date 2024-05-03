#include <cpr/cpr.h>
#include <iostream>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{std::string("https://api.spotify.com/v1/" ) + "me/player"},
                                    cpr::Header{{"Authorization", "Bearer " + std::string("BQBEZBwkmIqeogf0rFOJ9wswDW3vMw7Sb_xrVnXxQ74mzfKqO6ViAmEthtxVFgtf99wL7MEbh4ic_VvfIlV0G4k0ZO_EOKHxuYfEKXZYtu6bnLq_O5EVoKhTrlMj3oISS_b3EhkybLgQdu3fwyXUy8tFeE0L852aCDhIlAQQstyHrDEAyKMbM")}});
    std::cout << r.status_code;
}
