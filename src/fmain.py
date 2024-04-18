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

f = open(FOLDER + 'player.html', 'w')

template = """

<!DOCTYPE html>
<html>
<head>
    <title>fspot</title>
</head>
<body>
    <h1>well well well...</h1>
    <script src="https://sdk.scdn.co/spotify-player.js"></script>
    <button id="togglePlay">Toggle Play</button>
    <script>
        const params = new URLSearchParams(document.location.search);
        window.onSpotifyWebPlaybackSDKReady = () => {
            const token = params.get("access_token");
            const player = new Spotify.Player({
                name: 'fspot player',
                getOAuthToken: cb => { cb(token); },
                volume: 0.5 //TODO: change to the pickled value
            });

            // Ready
            player.addListener('ready', ({ device_id }) => {
                console.log('Ready with Device ID', device_id);
            });

            // Not Ready
            player.addListener('not_ready', ({ device_id }) => {
                console.log('Device ID has gone offline', device_id);
            });

            player.addListener('initialization_error', ({ message }) => {
                console.error(message);
            });

            player.addListener('authentication_error', ({ message }) => {
                console.error(message);
            });

            player.addListener('account_error', ({ message }) => {
                console.error(message);
            });

            document.getElementById('togglePlay').onclick = function() {
              console.log('BUTTON CLICKED');
              player.togglePlay();
            };
            player.pause().then(() => {
                console.log('Paused!');
            });

            player.connect().then(success => {
            if (success) {
                console.log('FSPOT successfully connected to Spotify!');
            }
            });


            player.connect();
            //window.open('','_self').close() //closes the window
        }
        document.getElementById("togglePlay").click();
        </script>
</body>
</html>

"""
f.write(template)
f.close()

#device_list = GET('me/player/devices').json()['devices']
#print(device_list)

webbrowser.open("file://" + FOLDER+'player.html?access_token=' + gl.auth_codes['access_token'])
change_player = threading.Thread(target=connect_player)
change_player.start()

loading_msg(change_player)
change_player.join()


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
    
