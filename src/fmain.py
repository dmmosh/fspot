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

buffer = ''
while(buffer != 'quit'):
    buffer = input('/ ')


    if buffer == 'play': # starts playback
        PUT('me/player/play')
    elif buffer == 'pause': # pauses playback
        PUT('me/player/pause')


#print(data.json())
    
