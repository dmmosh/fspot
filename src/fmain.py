from fglobal import *
from futils import *
from flogin import *
import futils
import fglobal as gl
import flogin as fl
'''
FSPOT
a lightweight spotify clients

manual compiling:
python -m PyInstaller --onefile src/fmain.py --name fspot

remove the pickled files:
rm ./fspot/*
'''

# BASICALLY MAIN

load_var = LOAD('auth.obj') # loads the authorization info
if load_var is None: # if there isnt any, open sign in screen
    login_start()


# loads from pickled 
gl.auth_codes = LOAD('auth.obj') 
gl.def_header = {  # sets the default header
            'Authorization': f'Bearer {gl.auth_codes["access_token"]}' 
}


refresh() # refreshes the token

data = GET('me') # testing

if data.status_code != 200: # if token is still invalid, rerun the login page
    login_start()
    data = GET('me')

#device_list = GET('me/player/devices').json()['devices']
#print(device_list)



#browser = Process(target=start_browser) # run as process rather than thread
#browser.start()


browser_localhost()


#os.system('kill -9 ' + app.applicationPid())

change_player = threading.Thread(target=connect_player) # runs connection to the player
change_player.start() # starts thread

loading_msg(change_player, msg="Connecting to the World Wide Web...  ") # starts the loading msg
change_player.join() # joins the thread to main
#clear_string(132) # clears the weird warning idk how to get rid of, dont change

buffer = '' # input command buffer
while(buffer != 'quit'):
    buffer = input('/ ')

    match buffer:
        case 'play': 
            requests.put('http://localhost:5000', json={'hello':'testing'})
            PUT('me/player/play')
        case 'pause': 
            PUT('me/player/pause')
        case 'print':
            device_list = GET('me/player/devices').json()['devices']
            print(device_list)
        case 'refresh':
            refresh(force=True)
            


#browser.terminate()
#print(data.json())
    
