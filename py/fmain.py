from fglobal import *
from futils import *
from flogin import *
from fuser import *
import fglobal as gl
'''
FSPOT
a lightweight spotify clients

 still in development, no running version YET

manual compiling:
TODO: compile on older glibc version in vm 
(cd librespot && cargo build --release) && cp ./librespot/target/release/librespot ./fspot
python -m PyInstaller --onefile src/fmain.py --name fspot

remove the pickled files:
rm ./fspot/*f

'''


def update_term():
    while(1):
        gl.term_size = os.get_terminal_size().columns
        time.sleep(0.05)

# ALWAYS UPDATES THE TERMINAL SIZE
threading.Thread(target=update_term, daemon=True).start()

# commands in prompt
for cmd in sys.argv:
    match cmd:
        case '--clear', '-c':
            os.system('clear')
        case '--reset', '-r':
            print('Force reset started...')
            login_start()
        case '--refresh', '-rf':
            # loads from pickled 
            gl.auth_codes = LOAD('auth.obj') 
            gl.def_header = {  # sets the default header
                        'Authorization': f'Bearer {gl.auth_codes["access_token"]}' 
            }
            print('Refreshing token...')
            refresh(force=True)
            me = GET('me') # testing
            if me.status_code == 200:
                print('Token refreshed.')
            else:
                ERROR('Could not refresh token.')


load_var = LOAD('auth.obj') # loads the authorization info
if load_var is None: # if there isnt any, open sign in screen
    login_start() # starts login
    

# loads from pickled 
gl.auth_codes = LOAD('auth.obj') 
gl.def_header = {  # sets the default header
            'Authorization': f'Bearer {gl.auth_codes["access_token"]}' 
}

refresh() # refreshes the token
me = GET('me') # testing

if me.status_code != 200: # if token is still invalid, rerun the login page
    login_start() # starts login


# FUNCTION CALLS WHEN PROGRAM ENDS
def end():
    PUT('me/player/pause')
    

    title = {'id': 1,   # the title slide properties
             'line_num': 3,
             'col_num': 10}
    # choose which title number to print (depending on column terminal size
    if gl.term_size < 42: # TITLE 1 (under 42)
        title = {'id': 1, 'line_num': 3, 'col_num': 10}
    elif gl.term_size < 52: # TITLE 2 (42 and over)
        title = {'id': 2, 'line_num': 9, 'col_num': 42}
    else: # TITLE 3 (52 and over)
        title = {'id': 3, 'line_num': 10, 'col_num': 52}
    

    title_text = open(FOLDER + 'titles/title' + str(title['id']) + '.txt', 'r')
    [ print( '\033[1m' + line + '\033[0m', end='') for line in title_text.readlines()]
    title_text.close()

    print('\n')
    move_up()

    print("\t" + TEXT['invert_on'] + "[ see ya... vro ]" + TEXT['invert_off'])

atexit.register(end)

program = threading.Thread(target=lambda: os.system(FOLDER+'librespot ' +
                    '--name \'fspot player\' ' +
                    '--disable-audio-cache ' +
                    '--disable-credential-cache ' +
                    '--device-type homething ' +
                    '-u \''+ gl.auth_codes['user_id'] +'\' ' +
                    '-p \'' + gl.auth_codes['password'] + '\' &> /dev/null '
                    ), daemon=True)

change_player = threading.Thread(target=connect_player, daemon=True) # runs connection to the player
change_player.start() # starts thread
loading_msg(change_player, msg="Connecting to the World Wide Web...  ") # starts the starting loading message
change_player.join() # joins the thread to mainsd




user_input()

# runs after quit command
#browser.terminate()
#print(data.json())
    
