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
        self.input = '' # user input

        # current variables (focus, etc)
        self.current = {'input': True, # whether there's input to take (turn on/off when switching tabs), change only in threads
                        'quit': False, # whether to quit or not (only when to quit the program)
                        'window': 'main' # the current focus window
        }


         # THREADS
        # all threads should be daemons
        self.keylog = Listener(on_press=self.on_press) # daemon on default
        self.main = threading.Thread(target=self.main_input, daemon=True)
        self.picker = threading.Thread(target=self.picker, daemon=True)


        # THREADS
        self.keylog.start()
        self.main.start()

        self.dummy() # dummy input function

    
    # on each key press
    def on_press(self, key:Key):
        match key:
            case Key.backspace:
                self.input = self.input[:-1]

            # ALL INPUT COMMANDS
            case Key.enter:
                match self.input_window:
                    case 'main':
                
                        match self.input:
                            case 'quit':
                                print('') # have to do this idk why 
                                self.input = False
                                self.quit = True
                            case 'search': 
                                print('')
                                self.input = False # exits the input loop
                                self.current['window'] = 'picker' #picker window

    
                print('\033[1A', end='\x1b[2K') # clears the current line
                self.input='' # wipes the user buffer
            case None:
                pass
            case _: # regular letterssd
                try:
                    self.input += key.char
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
        while(not self.quit and self.current['window'] == 'main'):
            if len(self.input) > 10:
                self.input = self.input[:10]

            print(self.input)
            print('', '\n/ ' + self.input, end='') # prints the initial line
            time.sleep(0.5) # waits a second

            print('', end='\x1b[2K') # clears current
            print('\033[1A', end='\x1b[2K') # moves up and clears
            clear_string(len(self.input))
            print('/ ' + self.input, end='') # moves cursor to the right
            print('') # prints newline
            delete_line(1) # deletes it (so lines in next iteration will start at beginning)

    def picker(self):
        pass




    



test = user_input()