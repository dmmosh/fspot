from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from pynput.keyboard import Key, Listener

user = '' # user input
quit = False
def on_press(key:Key) -> None:
    global user
    global quit 

    if len(user) > 10:
            user = user[:10]

    match key:
        case Key.backspace:
            user = user[:-1]
        case Key.enter:
            if user == 'q':
                print('') # have to do this idk why 
                quit = True
            user=''

            print('\033[1A', end='\x1b[2K') # clears the current line
        case None:
            pass
        case _: # regular letterssd
            try:
                user += key.char
            except:
                pass

def prints():
    global user
    global quit
    print('')
    delete_line()
    while(not quit):
        if len(user) > 10:
            user = user[:10]

        print(user)
        print('', '\n/ ' + user, end='') # prints the initial line
        time.sleep(1) # waits a second

        print('', end='\x1b[2K') # clears current
        print('\033[1A', end='\x1b[2K') # moves up and clears
        clear_string(len(user))
        print('/ ' + user, end='') # moves cursor to the right
        print('') # prints newline
        delete_line(1) # deletes it (so lines in next iteration will start at beginning)


Listener(on_press=on_press).start()

printing = threading.Thread(target=prints, daemon=True)
printing.start()

while (not quit):
    input() # dummy input


