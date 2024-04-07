from fglobal import *
from futils import *
from flogin import *
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

refresh()
data = GET('me')

if data.status_code != 200:
    login_start()
    data = GET('me')


players = GET('me/player/devices').json()
print(players)
for device in players:
    if 'fspot player' in device:
        print(device)
        break

device_list = GET('me/player/devices').json()['devices']
is_player_active = 0
for device in device_list:
    if device['name'] == 'fspot player':
        player = {'device_ids': [device['id']],
                  'play': False}
        print(player)
        request = PUT('me/player', json=player)
        print(request.status_code)
        PUT('me/player/pause')
        is_player_active = 1
        break
if is_player_active == 0:
    ERROR('Player inactive. Try restarting the app.')
    os._exit(0)

buffer = ''
while(buffer != 'quit'):
    buffer = input('/ ')

    match buffer:
        case 'play': 
            PUT('me/player/play')
        case 'pause': 
            PUT('me/player/pause')
        case 'print':
            device_list = GET('me/player/devices').json()['devices']
            print(device_list)

    

#print(data.json())
    
