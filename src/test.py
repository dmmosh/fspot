from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from pynput.keyboard import Key, Listener, KeyCode

user = '' # user input
quit = False
def on_click(key:Key) -> None:
    global user
    global quit 
    if key == Key.num_lock:
        pass
    elif key == Key.backspace:
        user = user[:-1]
    elif key == Key.enter:
        if user == 'q':
            print('/ q')
            quit = True
        user=''

        print('\033[1A', end='\x1b[2K')
    else:
        user += key.char

# starts the listener
Listener( on_press=on_click).start() # key listener

def prints():
    global user
    global quit
    i = 0
    print('')
    delete_line()
    while(not quit):
        print(i, '\n/ ' + user, end='') # prints the initial line
        time.sleep(1) # waits a second
        print('', end='\x1b[2K') # clears current
        print('\033[1A', end='\x1b[2K') # moves up and clears
        print('/ ' + user, end='') # moves cursor to the right
        print('') # prints newline
        delete_line() # deletes it (so lines in next iteration will start at beginning)
        i+=1



printing = threading.Thread(target=prints, daemon=True)
printing.start()


while (not quit):
    dummy = input() # dummy input
