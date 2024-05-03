# FSPOT #
A full-feature mini Spotify TUI client. Functions as both a controller (powered by the [Spotify Web API](https://developer.spotify.com/documentation/web-api)) and a player (by embedded [librespot](https://github.com/librespot-org/librespot)), with encrypted serialization and whatnot. In order words, a full Spotify player. Made because of the memory hog that is the default Spotify client and the frustration brought on by other 3rd party Spotify clients. 

Written in Python and C++. The main branch incorporates C++ as it is faster and far less "chunky" feeling than Python, especially when there are hundreds of strings being printed every minute, which, thanks to clever ANSI escape codes, mimic a TUI. There is, however, a Python branch if you want to try the chunky TUI. The Python branch, however, will not be receiving future updates because who wants a chunky TUI?

THe elephant in the room: Ncurses. Ncurses overrides the entire terminal screen. I do not want to do that because I want my player to be small and compact. Therefore, I have taken the long route of creating a functioning TUI on my own. 
