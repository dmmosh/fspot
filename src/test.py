from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from pynput.keyboard import Key, Listener


class user_input():
    user = ''
    quit = False
    input = True

    # inits everything
    def __init__(self) -> None:
        self.user = '' # user input
        self.quit = False # exits the program
        self.input = True # exits the user input (can be turned on/off)

         # THREADS
        # all threads should be daemons
        self.keylog = Listener(on_press=self.on_press) # daemon on default
        self.main = threading.Thread(target=self.main_input, daemon=True)


        # THREADS
        self.keylog.start()
        self.main.start()

        self.dummy() # dummy input function

    
    # on each key press
    def on_press(self, key:Key) -> None:
        match key:
            case Key.backspace:
                self.user = self.user[:-1]
            case Key.enter:
                match self.user:
                    case 'quit':
                        print('') # have to do this idk why 
                        self.input = False
                        self.quit = True
                    case 'search':
                        print('')
                        self.input = False
    
                print('\033[1A', end='\x1b[2K') # clears the current line
                self.user='' # wipes the user buffer
            case None:
                pass
            case _: # regular letterssd
                try:
                    self.user += key.char
                except:
                    pass
    


    # dummy loop to redirect input from terminal to python
    def dummy(self):
        while (self.input):
            input() # dummy input

    # the main input window
    def main_input(self):
        print('')
        delete_line()
        while(not quit):
            if len(self.user) > 10:
                self.user = self.user[:10]

            print(self.user)
            print('', '\n/ ' + self.user, end='') # prints the initial line
            time.sleep(0.5) # waits a second

            print('', end='\x1b[2K') # clears current
            print('\033[1A', end='\x1b[2K') # moves up and clears
            clear_string(len(self.user))
            print('/ ' + self.user, end='') # moves cursor to the right
            print('') # prints newline
            delete_line(1) # deletes it (so lines in next iteration will start at beginning)





    



test = user_input()