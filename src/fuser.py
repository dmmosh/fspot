from fglobal import *
from futils import *
import fglobal as gl

class user_input():

    # inits everything
    def __init__(self):
        self.buffer = '' # user input
        self.init_status = ['hello vro', 
                            'good morning', 
                            'i like waffles', 
                            'dinnerbone', 
                            'i love you vro..',
                            'hii!!! :3,',
                            'ready ?',
                            'hacked in the mainframe',
                            'haaaaiiii XD',
                            'long live vim',
                            'hi friend',
                            'pancakes or waffles?',
                            'hi',
                            'hello',
                            'hey',
                            'remember to stay HYDRATED',
                            'hoi',
                            'h',
                            'XD',
                            'vro..',
                            'get hacked bozooo',
                            'you should eat some PANCAKES',
                            'black metal?',
                            'owo']

        # current variables (focus, etc)
        self.current = { 'quit': False, # whether to quit or not from current, only change in threads
                          'logging': True, # whether to take input or not, ends threads
                            'window': 'main' # the current focus window
                        }


        self.status = {'message': '', 'sec': 0, 'blink': False}  # a message to print the user 

         # THREADS
        # all threads should be daemons
        #self.keylog = Listener(on_press=self.on_press) # daemon on default
        self.main = threading.Thread(target=self.main_input, daemon=True)
        self.search = threading.Thread(target=self.searcher, daemon=True)
        self.keylog = threading.Thread(target=self.key_log, daemon=True)

        # THREADS
        self.keylog.start()
        self.main.start()

        self.MESSAGE(self.init_status[randint(0,23)], blink=True)
            


        # redirect input from linux to python
        while(not self.current['quit']):
            pass



    # KEY LOGS USER INPUT 
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
                    if(len(self.buffer) > 15):
                        self.buffer = self.buffer[:16]

                    c = sys.stdin.read(3)
                    match c:
                        case '\x7f': # BACKSPACE
                            self.buffer = self.buffer[:-1]

                        case '\n': # NEWLINE CHARACTER
                            self.command = self.buffer
                            self.buffer = ''
                            sys.stdout.flush()
                            self.options(self.command) # calls options function

                        case ' ': # SPACE 
                            if GET('me/player').json()['is_playing']:
                                PUT('me/player/pause')
                            else: 
                                PUT('me/player/play')

                        case '\x1b[A': # UP KEY 
                            sys.stdout.write('\x1b[B\x1b[A')

                        case '\x1b[B': # DOWN KEY
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
        

    # decreases the counter , helper function to status
    def decrease(self, msg_len:int):
        self.temp = self.status['message']
        while(self.status['sec']):
            if self.status['blink']:
                for i in range(0,2):
                    self.status['message'] = '[ ' + (' '*msg_len) + ' ]'
                    time.sleep(0.5)
                    self.status['message'] = self.temp
            else:   
                time.sleep(1)
            self.status['sec']-=1
        self.status = {'message': '', 'sec': 0, 'blink': False}  # a message to print the user 
    

    # updates the status and runs it for the specified length of seconds
    def MESSAGE(self, message:str = 'Hello', sec:int = 5, blink:bool = False):
        # only print if it can match the length
        if (len(message) + 27) <= gl.term_size:
            self.status = {'message': '[ ' + message + ' ]' , 'sec': sec, 'blink': blink} # sets the status
            threading.Thread(target=self.decrease, args=(len(message))).start()

        


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
                if GET('me/player').json()['is_playing']:
                    PUT('me/player/pause')
                else: 
                    PUT('me/player/play')
            
            case 'print': # print devices
                device_list = GET('me/player/devices').json()
                print(device_list)

            case 'refresh': # refresh
                self.MESSAGE('Force token refresh')
                refresh(force=True)
            
            case 'clear': # if the screen gets all messed up
                os.system('clear')
            
            
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




    # the main input window
    def main_input(self):

        global LINE_CTR # the TOTAL lines counter 
        LINE_CTR = 4 

        # MOVES THE TERMINAL DOWN
        print('\n'*4, end='') 
        for i in range(0, LINE_CTR):
            move_up()                
            clear_line()
            
        progress = 0
        minute = 0
        second = 0
        progress = 0 # song progress bar
        while(self.current['logging']): # update

            
            loc = GET('me/player') # PLAYBACK COMMAND
            if (loc.status_code == 200):
                loc = loc.json()
                progress = (loc['progress_ms'] if loc['progress_ms'] else 0) // 1000
                minute = progress // 60
                second = progress % 60

            # TUI LINES 

            print('')
            print('PLAY STATUS:', str('0' + str(minute) if minute <10 else minute ) + ':' + str('0' + str(second) if second <10 else second ), self.buffer)


            # USER LINES

            if self.status['message'] != '' and len(self.status['message']) +23 <= gl.term_size:
                print('\n' + ' '*23 + self.status['message'])
                move_up(2)

            print('\n/ ' + self.buffer, end='') # prints the initial line

            time.sleep(0.2) # waits a second
            clear_line()
            move_up()
            
            if self.status['message'] != '' and len(self.status['message']) +23 <= gl.term_size:
                print('\n' + ' '*23 + self.status['message'])
                move_up(2)

            print('\n/ ' + self.buffer, end='') # prints the initial line
            
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



