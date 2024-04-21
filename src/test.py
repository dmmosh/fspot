from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from pynput.keyboard import Key, Listener, KeyCode

user = '' # user input
def on_click(key:Key) -> None:
    global user

    if key == Key.num_lock:
        pass
    elif key == Key.backspace:
        user = user[:-1]
    elif key == Key.enter:

        if user == 'q':
            os._exit(0)

        print('\033[1A', end='\x1b[2K')
        print('/ ' + user, end='')
    else:
        user += key.char

# starts the listener
Listener( on_press=on_click).start() # key listener

def prints():
    global user
    for i in range(0,20):
        print(i, '\n/ ' + user, end='')
        time.sleep(0.3)
        print('', end='\x1b[2K')
        print('\033[1A', end='\x1b[2K')
        print('/ ' + user, end='')




prints()
