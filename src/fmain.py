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

        window.onSpotifyWebPlaybackSDKReady = () => {
            const token = '"""+ gl.auth_codes['access_token'] +"""';
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
              player.togglePlay();
            };
            player.pause().then(() => {
                console.log('Paused!');
            });

            player.connect().then(success => {
              if (success) {
                player.togglePlay();
                console.log('SPOTIFY CONNECTED');
              }
            });
            player.getCurrentState().then(state => {
              if (!state) {
                console.log('User is not playing music through fspot');
                return;
              }
                player.togglePlay();
            });


            player.connect();
            //window.open('','_self').close() //closes the window
        }
        </script>
</body>
</html>

"""
f.write(template)
f.close()

webbrowser.open(FOLDER+'player.html')
device_list = GET('me/player/devices').json()['devices']
print(device_list)

timer = 120 # 30 seconds, 
dot_ctr = 1 # this modulus 3 gives the loading dots
while(timer):
    device_list = GET('me/player/devices').json()['devices']
    for device in device_list:
        if device['name'] == 'fspot player':
            player = {'device_ids': [device['id']],
                      'play': True}
            request = PUT('me/player', json=player)
            if (GET('me/player').status_code == 204):
                break
    
    sys.stdout.write('\r' + 'Connecting to the World Wide Web' + ('.' *(dot_ctr%3)))
    sys.stdout.flush()
    dot_ctr += 1
    time.sleep(0.25)
        

if timer == 0: # request took too long
    ERROR('Request took too long. Maybe get better internet.')

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
    
