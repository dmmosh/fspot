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
        self.current = { 'quit': False, # whether to quit or not from current, only change in threads
                        'window': 'main' # the current focus window
                        }
        

         # THREADS
        # all threads should be daemons
        self.keylog = Listener(on_press=self.on_press) # daemon on default
        self.main = threading.Thread(target=self.main_input, daemon=True)
        self.search = threading.Thread(target=self.searcher, daemon=True)



        # THREADS
        self.keylog.start()
        self.main.start()

        self.dummy() # dummy input function

    # options menu (to minimize nesting)
    # ONLY CALL WHEN ENTER KEY IS CALLED
    def options(self, command:str = 'quit'):
        # NON-TERMINAL SPECIFIC OPTIONS


        match command:
            case 'quit':
                print('')
                self.current['quit'] = True
            case 'search': 
                self.current['quit'] = True
                self.current['quit'] = False
                self.current['window'] = 'search' #picker window
                self.search.start()
                self.dummy()

    # on each key press
    def on_press(self, key:Key):
        match key:
            case Key.backspace:
                self.buffer = self.buffer[:-1]

            # ALL INPUT COMMANDS
            case Key.enter:
                command = self.buffer
                self.buffer = ''
                print('\033[1A', end='\x1b[2K')
                self.options(command) # calls options function
            case None:
                pass
            case _: # regular letterssd
                try:
                    self.buffer += key.char
                except:
                    pass
    


    # dummy loop to redirect input from terminal to python
    def dummy(self):
        while (not self.current['quit']):
            input() # dummy input

    # the main input window
    def main_input(self):
        print('')
        delete_line()
        while(not self.current['quit']):
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

    def searcher(self):
        print('\n\n\n\n')
        delete_line()
        while(not self.current['quit']):
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