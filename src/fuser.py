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
                          'clear': False,
                            'window': 'main' # the current focus window
                        }

        

        self.status = {'message': '', 'sec': 0}  # a message to print the user 

         # THREADS
        # all threads should be daemons
        #self.keylog = Listener(on_press=self.on_press) # daemon on default
        self.main = threading.Thread(target=self.main_input, daemon=True)
        self.search = threading.Thread(target=self.searcher, daemon=True)
        self.keylog = threading.Thread(target=self.key_log, daemon=True)

        # THREADS
        self.keylog.start()
        self.main.start()

        self.MESSAGE(INIT_STATUS[randint(0,23)], 30) # prints welcome message
            


        # redirect input from linux to python
        # keeps repeating for most of the program
        while(not self.current['quit']):
            pass


    # KEY LOGS USER INPUT 
    # options menu (to minimize nesting)
    # ONLY CALL WHEN ENTER KEY IS CALLED
    def options(self, command:str):
        # NON-TERMINAL SPECIFIC OPTIONS

        match command:
            case 'start': # start
                PUT('me/player/play')

            case 'pause':  # pause
                PUT('me/player/pause')

            case 'play':  # start/pause
                if GET('me/player').json()['is_playing']: # if playing
                    self.PAUSE()
                else: 
                    self.PLAY()
            
            case 'print': # print devices
                device_list = GET('me/player/devices').json()
                print(device_list)

            case 'refresh': # refresh
                self.REFRESH()
            
            case 'clear': # if the screen gets all messed up
                self.current['clear'] = True

            
            case 'quit': # quits the user input
                # exits the class's constructor
                self.MESSAGE('Quitting...')
                self.current['logging'] = False
                self.current['quit'] = True 

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
                        clear_line()
                        self.current['quit'] = True
                        self.keylog.stop()
                    case 'search':
                        self.current['logging'] = False
                        self.current['logging'] = True
                        #self.curr_input.join()
                        self.current['window'] = 'main' #picker window
                        self.main.join()
                        self.main.start()



    # HELPER FUNCTIONS

    # important key log
    def key_log(self):
        self.fd = sys.stdin.fileno()

        self.oldterm = termios.tcgetattr(self.fd)
        self.newattr = termios.tcgetattr(self.fd)
        self.newattr[3] = self.newattr[3] & ~termios.ICANON & ~termios.ECHO
        termios.tcsetattr(self.fd, termios.TCSANOW, self.newattr)

        self.oldflags = fcntl.fcntl(self.fd, fcntl.F_GETFL)
        fcntl.fcntl(self.fd, fcntl.F_SETFL, self.oldflags | os.O_NONBLOCK)

        try:
            while (not self.current['quit']):
                
                try:
                    

                    c = sys.stdin.read(3) # reads 3 chars at a time
                    match c:
                        case '\x7f': # BACKSPACE
                            self.buffer = self.buffer[:-1]

                        case '\n': # NEWLINE CHARACTER
                            self.command = self.buffer
                            self.buffer = ''
                            sys.stdout.flush()
                            self.options(self.command) # calls options function

                        case ' ': # SPACE 
                            if GET('me/player').json()['is_playing']: # if playing right now
                                self.PAUSE()
                            else: 
                                self.PLAY() # if not

                        case '\x1b[A': # UP KEY , blocks it
                            sys.stdout.write('\x1b[B\x1b[A')

                        case '\x1b[B': # DOWN KEY, blocks it
                            sys.stdout.write('\x1b[A\x1b[B')

                        case '\x1b[D':
                            self.MESSAGE('left', 1)
                            sys.stdout.write('\x1b[D')

                        case '\x1b[C':
                            self.MESSAGE('right', 1)
                            sys.stdout.write('\x1b[D')

                        case None:
                            pass

                        case _:
                            try: # NORMAL CHARACTER
                                self.buffer += c  
                            except:
                                pass

                except IOError: pass
        finally:
            termios.tcsetattr(self.fd, termios.TCSAFLUSH, self.oldterm)
            fcntl.fcntl(self.fd, fcntl.F_SETFL, self.oldflags)
        

    # HELPER FUNCTIONS

    # pause helper function, comes up a lot
    def PAUSE(self):
        self.MESSAGE('Pausing...')
        PUT('me/player/pause')
        self.STOP_MESSAGE()
        self.MESSAGE('Paused now!', 2)
    

    # play helper function, comes up a lot
    def PLAY(self):
        self.MESSAGE('Playing...')
        PUT('me/player/play')
        self.STOP_MESSAGE()
        self.MESSAGE('Playing now!', 2)
    
    # refreshes the token, automated and manual
    def REFRESH(self):
        self.MESSAGE('Token refreshing...')
        refresh(force=True)
        self.STOP_MESSAGE()
        if GET('me').status_code == 200:
            self.MESSAGE('Token refreshed.', 2)
        else:
            self.MESSAGE('Error in refreshing token,,', 2)

    # decreases the counter , helper function to status
    def DECREASE(self):
        while(self.status['sec']):
            time.sleep(1)
            self.status['sec']-=1
        self.STOP_MESSAGE()



    

    # updates the status and runs it for the specified length of seconds
    def MESSAGE(self, message:str = 'Hello', sec:int = 5)-> threading.Thread:
        self.STOP_MESSAGE()
        # only print if it can match the length
        if len(message)+4 <= gl.term_size:
            self.status = {'message': TEXT['invert_on'] + '[ ' + message + ' ]' + TEXT['invert_off'], 'sec': sec} # sets the status
            threading.Thread(target=self.DECREASE, daemon=True).start() # returns thread (if needs to join later)

    # stops message when some process finishes
    def STOP_MESSAGE(self):
        self.status = {'message': '', 'sec': 0}




    # WINDOWS

    # the main input window
    def main_input(self):

        global LINE_CTR # the TOTAL lines counter 
        LINE_CTR = 5

        # MOVES THE TERMINAL DOWN
        print('\n'*4, end='') 
        for i in range(0, LINE_CTR):
            move_up()                
            clear_line()
            
        
        song = {'progress': 0,
                        'minute': 0,
                        'second': 0,
                        'progress': 0,
                        'percent': 0,
                        'artists': 'No one',
                        'name': 'Nothing duh',
                        'title': 'No one : Nothing duh'}

        while(self.current['logging']): # update
            if (self.current['clear']):
                lines = 10
                print('\x1b[E')
                for i in range(0,lines):
                    move_up()
                    clear_line()
                    time.sleep(0.01)
                
                self.current['clear'] = False

            if(len(self.buffer) > 15):
                self.buffer = self.buffer[:16]
            
            loc = GET('me/player') # PLAYBACK COMMAND
            if (loc.status_code == 200):
                loc = loc.json()

                if loc['progress_ms']: # if theres a song playing, set properties
                    

                    song['progress'] = loc['progress_ms'] // 1000

                    song['minute'] = song['progress'] // 60
                    song['second'] = song['progress'] % 60

                    song['percent'] = loc['progress_ms'] / loc['item']['duration_ms']
                    song['name'] = loc['item']['name']

                    song['artists'] = ' - '.join([ artist['name'] for artist in loc['item']['artists']])
                    if len(song['artists']) > gl.term_size//2 -10: # if artists title is too long, cut it
                        song['artists'] = song['artists'][:gl.term_size//2 -3] # cuts it
                        song['artists'] += '...' # adds  3 dots
                    
                    # checks if the length is fine                      -3 from soon to be colon separator and 10 for padding
                    if len(song['name']) > gl.term_size - len(song['artists']) - 13: # if song is too long, cut it
                        song['name'] = song['name'][:gl.term_size - len(song['artists']) - 6] # cuts it
                        song['name'] += '...' # adds  3 dots


                    
                    song['title'] = song['artists'] + ' : ' + song['name']

                    

            # TUI LINES 
            print('')
            print(CENTER(song['title']))
            

            bar = '-'* (gl.term_size-20 )
            to_print = int(len(bar)*song['percent'])

            bar = '<'+ bar[:to_print] + ' '*(len(bar)-to_print) + '>'


            print( TEXT['bold_on'] + CENTER(bar) + TEXT['bold_off'])


            move_up()
            print(str('0' + str(song['minute']) if song['minute'] <10 else song['minute'] ) + ':' + str('0' + str(song['second']) if song['second'] <10 else song['second'] ))


            # the progress bar, 7 spaces of padding on both sides




            # USER LINES    
            print('\n' + CENTER(self.status['message']))
            move_up(2)
            print('\n' + TEXT['invert_on'] + '// ' + self.buffer + TEXT['invert_off'] , end='') # prints the initial line
            time.sleep(0.2) # waits a second
            clear_line() # makes sure the user experience is nice
            move_up() # moves up

            print('\n' + CENTER(self.status['message']))
            move_up(2) # moves up again

            print('\n' + TEXT['invert_on'] + '// ' + self.buffer + TEXT['invert_off'] , end='') # prints the initial line

            move_up()
            for i in range(0,LINE_CTR-2): # amount of lines to clear
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



