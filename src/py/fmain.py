from fglobal import *
from futils import *
from flogin import *
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

for arg in sys.argv[1:]:
    if arg == "--help" or arg == "-h":
        if os.path.exists(FOLDER+'help.txt'):
            print(open(FOLDER+'help.txt', 'r').read())
        


# ALWAYS UPDATES THE TERMINAL SIZE
#threading.Thread(target=update_term, daemon=True).start()


load_var = LOAD('auth.obj') # loads the authorization info
if load_var is None: # if there isnt any, open sign in screen
    login_start() # starts login
    

# loads from pickled 
gl.auth_codes = LOAD('auth.obj') 
gl.def_header = {  # sets the default header
            'Authorization': f'Bearer {gl.auth_codes["access_token"]}' 
}

refresh(force=True) # refreshes the token
me = GET('me') # testing


if me.status_code != 200: # if token is still invalid, rerun the login page
    login_start() # starts login

# fspot/librespot --name 'fspot player' --disable-audio-cache --disable-credential-cache -u '' -p ''
program = subprocess.Popen([FOLDER + 'librespot',
                    '--name', 'fspot player',
                    '--disable-audio-cache',
                    '--emit-sink-events',
                    '--disable-credential-cache',
                    '-u', gl.auth_codes['user_id'],
                    '-p',  gl.auth_codes['password'], 
                    '&>', '/dev/null'],
                    stdout=subprocess.PIPE, stderr=subprocess.PIPE)

#atexit.register(lambda:os.killpg(os.getpgid(program.pid), signal.SIGKILL))
#atexit.register(end)

me_volume = GET('me/player')
if (me_volume.status_code == 200):
    me_volume = me_volume.json()['device']['volume_percent']
else:
    me_volume = 83


change_player = threading.Thread(target=connect_player, daemon=True) # runs connection to the player
change_player.start() # starts thread
#print(GET('me/player/devices').json())
erase_num = loading_msg(change_player, msg="Connecting to the World Wide Web...  ") # starts the starting loading message
change_player.join() # joins the thread to mainsd

#player = subprocess.Popen([FOLDER + 'fplayer', # fplayer executable
#                           str(erase_num),
#                            # basic encryption of files
#                           base64.b64encode(gl.auth_codes['access_token'].encode("ascii") ),
#                           base64.b64encode(gl.auth_codes['refresh_token'].encode("ascii") ),
#                           base64.b64encode(str(int(gl.auth_codes['expires_at'])).encode("ascii"))])

#player.wait()

PUT('me/player/volume?volume_percent='+ str(me_volume)) # sets the volume

atexit.register(lambda:os.system("{}fplayer {} {} {} {} {}".format(
                            FOLDER,
                           str(erase_num),
                           base64.b64encode(gl.auth_codes['access_token'].encode("ascii") ).decode(),
                           base64.b64encode(gl.auth_codes['refresh_token'].encode("ascii") ).decode(),
                           base64.b64encode(str(int(gl.auth_codes['expires_at'])).encode("ascii")).decode(),
                           base64.b64encode(str(int(program.pid)).encode("ascii") ).decode()
                           )))