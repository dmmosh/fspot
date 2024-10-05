<pre>
<strong> ________ ________  ________  ________  _________   </strong>  
<strong>|\  _____\\   ____\|\   __  \|\   __  \|\___   ___\ </strong>  
<strong>\ \  \__/\ \  \___|\ \  \|\  \ \  \|\  \|___ \  \_| </strong>  
<strong> \ \   __\\ \_____  \ \   ____\ \  \\\  \   \ \  \  </strong>  
<strong>  \ \  \_| \|____|\  \ \  \___|\ \  \\\  \   \ \  \ </strong>  
<strong>   \ \__\    ____\_\  \ \__\    \ \_______\   \ \__\</strong>  
<strong>    \|__|   |\_________\|__|     \|_______|    \|__|</strong>  
<strong>            \|_________|                            </strong>  
</pre>

FSPOT: A full-feature mini Spotify TUI client. Functions as both a controller (powered by the [Spotify Web API](https://developer.spotify.com/documentation/web-api)) and a player (by embedded [librespot](https://github.com/librespot-org/librespot)), with encrypted serialization and whatnot. In order words, a full Spotify player. Made because of the memory hog that is the default Spotify client and the frustration brought on by other 3rd party Spotify clients. 

Written in Python and C++. The main branch incorporates C++ as it is faster and far less "chunky" feeling than Python, especially when there are hundreds of strings being printed every minute, which, thanks to clever ANSI escape codes, mimic a TUI. There is, however, a Python branch if you want to try the chunky TUI. The Python branch, however, will not be receiving future updates because who wants a chunky TUI?

The elephant in the room: Ncurses. Ncurses overrides the entire terminal screen. I do not want to do that because I want my player to be small and compact. Therefore, I have taken the long route of creating a functioning TUI on my own. 

## Installation: ##
- Librespot (v5.0.0) (dev branch as of 10/05/2024)


## Installation: ##
```
# No proper installation script as of yet.
```

## Uninstallation: ##
```
# No proper uninstallation script as of yet.
```

## Manual Compiling: ##
There are a lot of libraries that need to be present. The installation scripts copy precompiled executables so there's no need for acquiring a million libraries.
```
python -m PyInstaller src/py/fmain.py --onefile --name fstart && cp dist/fstart fspot/
g++ -lcpr -std=c++20 src/main.cc src/utils.cc -o fspot/fplayer 
```

## Pretty Pictures: ##
![2024-05-03_12-34](https://github.com/dmmosh/fspot/assets/119987092/3610abee-8a7c-435c-a3ae-a9142e5e8573)
![2024-05-03_12-34_1](https://github.com/dmmosh/fspot/assets/119987092/49ac9ee2-31d0-438e-8600-0a1dccecbf3a)
![2024-05-03_12-33](https://github.com/dmmosh/fspot/assets/119987092/45c2f35b-5c30-483f-8cd2-b4df4a5c0bca)
![2024-05-03_12-36_1](https://github.com/dmmosh/fspot/assets/119987092/f1d4b921-46a5-45dc-87dc-337403c26185)





