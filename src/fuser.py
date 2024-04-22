from fglobal import *
from futils import *
import fglobal as gl

class user_input():

    # inits everything
    def __init__(self):
        self.buffer = '' # user input

        # current variables (focus, etc)
        self.current = { 'quit': False, # whether to quit or not from current, only change in threads
                          'logging': True, # whether to take input or not, ends threads
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

        # redirect input from linux to python
        while(not self.current['quit']):
            input() # dummy input (to not go to )



    # options menu (to minimize nesting)
    # ONLY CALL WHEN ENTER KEY IS CALLED
    def options(self, command:str = 'quit'):
        # NON-TERMINAL SPECIFIC OPTIONS


        match command:
            case 'start': # start
                PUT('me/player/play')

            case 'pause':  # pause
                PUT('me/player/pause')

            case 'play':  # start/pause
                if GET('me/player').json()['is_playing']:
                    PUT('me/player/pause')
                else: 
                    PUT('me/player/play')
            
            case 'print': # print devices
                device_list = GET('me/player/devices').json()
                print(device_list)

            case 'refresh': # refresh
                refresh(force=True)
            
            case 'player_end': # end the player
                end_player()
                return
            
            case 'quit': # quits the user input
                print('')
                self.current['quit'] = True
                self.keylog.stop()
            case 'search':  # goes to the lower search bar
                self.current['logging'] = False
                self.current['logging'] = True
                #self.curr_input.join()
                self.current['window'] = 'search' #picker window
                self.search.start()
            case 'main': 
                self.current['logging'] = False
                self.current['logging'] = True
                #self.curr_input.join()
                self.current['window'] = 'main' #picker window
                self.main.start()
            case 'back': #WIP TODO: finish this
                match self.current['window']:
                    case 'main':
                        print('')
                        self.current['quit'] = True
                        self.keylog.stop()
                    case 'search':
                        self.current['logging'] = False
                        self.current['logging'] = True
                        #self.curr_input.join()
                        self.current['window'] = 'main' #picker window
                        self.main.join()
                        self.main.start()


    # on each key press
    def on_press(self, key:Key):
        match key:
            case Key.backspace:
                self.buffer = self.buffer[:-1]
            # ALL INPUT COMMANDS
            
            case Key.enter:
                command = self.buffer
                self.buffer = ''
                move_up()
                clear_line()
                self.options(command) # calls options function
            case None:
                pass
            case _: # regular letterssd
                try:
                    self.buffer += key.char
                except:
                    pass



    # the main input window
    def main_input(self):
        print('\n'*5, end='')
        for i in range(0,5):
            move_up()                
            clear_line()
            
        global progress
        global print_ctr

        progress = 0 # song progress bar
        print_ctr = 0
        while(self.current['logging']): # update



            loc = GET('me/player')
            if (loc.status_code == 200):
                loc = loc.json()
                progress = loc['progress_ms'] if loc['progress_ms'] else 0
            
            
            print("") # debug
            print('hello')
            print(progress) # debug
            print('\n/ ' + self.buffer, end='') # prints the initial line

            time.sleep(0.5) # waits a second
            clear_line()
            move_up()
            print('\n/ ' + self.buffer, end='') # prints the initial line
            
            move_up()
            for i in range(0,3):
                move_up()
                clear_line()

            
            
            

    def searcher(self):
        print('\n\n\n\n')
        clear_line()
        while(self.current['logging']):
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
            clear_line(1) # deletes it (so lines in next iteration will start at beginning)
        
        # EXIT CONDITION maybe
        print('fdjdfk')



