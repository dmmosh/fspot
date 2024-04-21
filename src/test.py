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
        print(i, '\n/ ' + user, end='')
        time.sleep(1)
        print('', end='\x1b[2K')
        print('\033[1A', end='\x1b[2K')
        print('/ ' + user, end='')
        print('')
        delete_line()
        i+=1



printing = threading.Thread(target=prints(), daemon=True)
printing.start()


while(not quit):
    dummy = input()

