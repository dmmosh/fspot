from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from pynput.keyboard import Key, Listener


class user_input():

    # inits everything
    def __init__(self):
        self.buffer = '' # user input

        # current variables (focus, etc)
        self.current = {'input': True, # whether there's input to take (turn on/off when switching tabs), change only in threads
                        'quit': False, # whether to quit or not (only when to quit the program)
                        'window': 'main' # the current focus window
        }
        

         # THREADS
        # all threads should be daemons
        self.keylog = Listener(on_press=self.on_press) # daemon on default
        self.main = threading.Thread(target=self.main_input, daemon=True)
        self.pick = threading.Thread(target=self.picker, daemon=True)



        # THREADS
        self.keylog.start()
        self.main.start()

        self.dummy() # dummy input function

    # options menu (to minimize nesting)
    # ONLY CALL WHEN ENTER KEY IS CALLED
    def options(self):
        # NON-TERMINAL SPECIFIC OPTIONS

        match self.buffer:
            case 'quit':
                print('') # have to do this idk why 
                self.current['input'] = False
                self.current['quit'] = True
            case 'search': 
                print('njdkdfhsdhjk')
                self.current['input'] = False # exits the input loop
                self.current['window'] = 'picker' #picker window
                self.pick.start()

        print('\033[1A', end='\x1b[2K') # clears the current line
        self.buffer='' # wipes the user buffer
    # on each key press
    def on_press(self, key:Key):
        match key:
            case Key.backspace:
                self.buffer = self.buffer[:-1]

            # ALL INPUT COMMANDS
            case Key.enter:
                self.options() # calls options function
            case None:
                pass
            case _: # regular letterssd
                try:
                    self.buffer += key.char
                except:
                    pass
    


    # dummy loop to redirect input from terminal to python
    def dummy(self):
        while (self.current['input']):
            input() # dummy input

    # the main input window
    def main_input(self):
        print('')
        delete_line()
        while(not self.current['quit'] and self.current['window'] == 'main'):
            if len(self.buffer) > 10:
                self.buffer = self.buffer[:10]

            print(self.buffer) # debug
            print('', '\n/ ' + self.buffer, end='') # prints the initial line
            time.sleep(0.5) # waits a second

            print('', end='\x1b[2K') # clears current
            print('\033[1A', end='\x1b[2K') # moves up and clears
            clear_string(len(self.buffer))
            print('/ ' + self.buffer, end='') # moves cursor to the right
            print('') # prints newline
            delete_line(1) # deletes it (so lines in next iteration will start at beginning)

    def picker(self):
        print('')
        delete_line()
        while(not self.current['quit'] and self.current['window'] == 'search'):
            if len(self.buffer) > 10:
                self.buffer = self.buffer[:10]

            print(self.buffer) # debug
            print('', '\n/ ' + self.buffer, end='') # prints the initial line
            time.sleep(0.5) # waits a second

            print('', end='\x1b[2K') # clears current
            print('\033[1A', end='\x1b[2K') # moves up and clears
            clear_string(len(self.buffer))
            print('/ ' + self.buffer, end='') # moves cursor to the right
            print('') # prints newline
            delete_line(1) # deletes it (so lines in next iteration will start at beginning)




    



test = user_input()