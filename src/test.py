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


Listener(on_press=on_press).start()

def prints():
    global user
    global quit
    i = 0
    print('')
    delete_line()
    while(not quit):
        print(user)
        print('', '\n/ ' + user, end='') # prints the initial line
        time.sleep(1) # waits a second

        print('', end='\x1b[2K') # clears current
        print('\033[1A', end='\x1b[2K') # moves up and clears
        clear_string(len(user))
        print('/ ' + user, end='') # moves cursor to the right
        print('') # prints newline
        delete_line() # deletes it (so lines in next iteration will start at beginning)

# dummy input, captures it so the terminal doesnt
def dummy():
    global quit
    while (not quit):
        input() # dummy input


printing = threading.Thread(target=prints, daemon=True)
printing.start()

dummy_input = threading.Thread(target=dummy, daemon=True)
dummy_input.start()


