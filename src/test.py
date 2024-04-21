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
        user=''

        print('\033[1A', end='\x1b[2K')
    else:
        user += key.char

# starts the listener
Listener( on_press=on_click).start() # key listener

def prints():
    global user
    print('\n\n\n')
    delete_line(4)
    for i in range(0,10):
        print(i, '\n/ ' + user, end='')
        time.sleep(1)
        print('', end='\x1b[2K')
        print('\033[1A', end='\x1b[2K')
        print('/ ' + user, end='')
        print('\n\n\n')
        delete_line(4)



prints()
