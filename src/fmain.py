from fglobal import *
from futils import *
from flogin import *
from fuser import *
import fglobal as gl
import signal
'''
FSPOT
a lightweight spotify clients

manual compiling:
python -m PyInstaller --onefile src/fmain.py --name fspot

remove the pickled files:
rm ./fspot/*
'''



# commands in prompt
for cmd in sys.argv:
    match cmd:
        case '--reset':
            login_start()


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



player = subprocess.Popen(['librespot ',
                                '--name \"fspot player\" ',
                                '--disable-audio-cache ',
                                '--disable-credential-cache ',
                                '--device-type homething ',
                                '-u \"'+ gl.auth_codes['user_id'] +'\" ',
                                '-p \"' + gl.auth_codes['password'] + '\"' ],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)

change_player = threading.Thread(target=connect_player, daemon=True) # runs connection to the player
change_player.start() # starts thread
loading_msg(change_player, msg="Connecting to the World Wide Web...  ") # starts the loading msg
change_player.join() # joins the thread to main


user = user_input()

os.killpg(os.getpgid(player.pid), signal.SIGTERM)
#browser.terminate()
#print(data.json())
    
