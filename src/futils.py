from fglobal import *
import fglobal as gl


# USER INPUT FUNCTION
def user_input():
    buffer = '' # input command buffer
    while(1):
        buffer = input('/ ')
        clear_string(len('/ ' + buffer)) # clears the string bufferf
        match buffer:
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
                refresh(force=True)
            case 'quit': # quit program
                return
            case 'player_end': # end the player
                end_player()
                return
                



# PICKLING
# saves the pickle object to var folder
def SAVE(to_save, var_name:str): # pickle saving variables
    if not os.path.exists(FOLDER+ 'var/'): # if the variable directory doesnt exist, make one
        os.mkdir(FOLDER+ 'var/') # makes directory
    pickle.dump(to_save, open(FOLDER+ 'var/' + var_name, 'wb')) # dumps the file
    pyAesCrypt.encryptFile(FOLDER+'var/'+var_name, FOLDER+'var/'+var_name + '.aes', PASSWORD) # encrypts the file
    os.remove(FOLDER+ 'var/' + var_name) # removes the folder

# loads the pickle object from the var folder
def LOAD(var_name:str): # pickle loading variables
    if not os.path.exists(FOLDER+ 'var/'): # if the variable directory doesnt exist, make one
        os.mkdir(FOLDER+ 'var/') # makes directory
    
    if os.path.exists(FOLDER+ 'var/' + var_name + '.aes'): # if variable exists
        pyAesCrypt.decryptFile(FOLDER+'var/'+var_name+'.aes', FOLDER+'var/'+var_name, PASSWORD) #decrypts the file
        output = pickle.load(open(FOLDER+ 'var/' + var_name, 'rb'))  # loads the decrypted file
        os.remove(FOLDER+ 'var/' + var_name) # deletes the decrypted file
        return output # returns the decrypted file
    else: # if it doesnt, returns nothing
        return None
    

# HELPER FUNCTIONS

# fatal error
def ERROR(*args:str)->None: # prints error message
    print("\n\nFATAL ERROR:") 
    for to_print in args:
        print('\t' + to_print)
    os._exit(0)

# adds the authorization code to the header
def HEADER(request:dict = None)-> dict: # returns a dict
    return gl.def_header if request is None else request.update(gl.def_header)
    # returns just the default header if empty OR the request with the def header



def GET(where_from:str, params:dict = None, data:dict = None, json:dict = None, headers:dict = None): # a get request, retrieves resources
    return requests.get(BASE_URL + where_from,
                         params=params,
                         data=data,
                         json=json,
                         headers=HEADER(headers), 
                         allow_redirects=True )

def PUT(where_from:str, params:dict = None, data:dict = None, json:dict = None, headers:dict = None): # a post request, creates resources
    return requests.put(BASE_URL + where_from, 
                         params=params,
                         data=data,
                         json=json,
                         headers=HEADER(headers), 
                         allow_redirects=True)

def POST(where_from:str, params:dict = None, data:dict = None, json:dict = None, headers:dict = None): # a put request, changes and/or replaces resources
    return requests.post(BASE_URL + where_from, 
                          params=params,
                          data=data,
                          json=json,
                          headers=HEADER(headers), 
                          allow_redirects=True)

def DELETE(where_from:str, params:dict = None, data:dict = None, json:dict = None, headers:dict = None): # a delete request, deletes resources
    return requests.delete(BASE_URL + where_from, 
                            params=params,
                            data=data,
                            json=json,
                            headers=HEADER(headers), 
                            allow_redirects=True)


# HELPER FUNCTIONS

# a loading message, clears itself when finishes
def loading_msg(process:threading.Thread, msg:str = 'Loading...')-> None:
    print_msg = msg
    while(process.is_alive()):
        for char in "/—\|":
            time.sleep(0.15)
            term_col = os.get_terminal_size().columns # gets column size
            print_msg = msg[:term_col-7] + '...  ' if (len(msg)+1 > term_col) else msg # shortens the print message if need be
            sys.stdout.write(print_msg + char + '\r')
            sys.stdout.flush()
    print('')
    clear_string(len(print_msg))

    #print("\r") # carriage return
    

def start_player():

    player = subprocess.Popen('librespot ' + 
                                '--name \"fspot player\" ' +
                                '--disable-audio-cache ' +
                                '--disable-credential-cache ' +
                                '--device-type homething ' +
                                '-u \"'+ gl.auth_codes['user_id'] +'\" ' +
                                '-p \"' + gl.auth_codes['password'] + '\"',
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    
    SAVE(player.pid, 'player.obj')


    change_player = threading.Thread(target=connect_player) # runs connection to the player
    change_player.start() # starts thread

    loading_msg(change_player, msg="Connecting to the World Wide Web...  ") # starts the loading msg
    change_player.join() # joins the thread to main
    #clear_string(132) # clears the weird warning idk how to get rid of, dont change

def connect_player():
    timer = 60 # 30 seconds (iterates every half a second)
    while(timer):
        
        device_list = GET('me/player/devices').json()
        if 'devices' not in device_list:
            ERROR('Something else happened. Failed to connect player.')

        for device in device_list['devices']:
            if device['name'] == 'fspot player':
                player = {'device_ids': [device['id']],
                          'play': False}
                
                request = PUT('me/player', json=player)

                if(request.status_code == 204): # exits the function
                    time.sleep(1) # wait an extra second for the put request
                    return
        
        time.sleep(0.5)
        timer-=1
        
    # once timer runs out
    ERROR('Request took too long. Maybe get better internet.', 'You might have set the wrong password. Run \"fpost --reset\"')


# end player
def end_player():
    player = GET('me/player')

    # if the player is already playing in the background
    if player.status_code == 200: # makes sure the fspot player is there
        player = player.json()['device']
        if player['name'] == 'fspot player' and player['is_active']:
            pid = LOAD('player.obj')
            if pid == None:
                ERROR('No pid file found. Please reboot your system for the playback to stop.')
            os.system('kill -9 ' + str(LOAD('player.obj')))
            return

            
        



# DELETES LAST LINE
def delete_line(n:int = 1)-> None:
    while(n):
        sys.stdout.write('\x1b[1A')
        sys.stdout.write('\x1b[2K')
        n-=1

# clears the string length from the printed lines (every char after newline)
def clear_string(strlen:int)->None:
    delete_line(math.ceil(strlen / os.get_terminal_size().columns) if (strlen > 0) else 1)

# REFRESH THE ACCESS TOKEN
def refresh(force:bool = False):
    if (datetime.now().timestamp() > gl.auth_codes['expires_at']) or force:
        print("Token expired. Refreshing.")
        req_headers = {
            'Content-Type': 'application/x-www-form-urlencoded',
            'Authorization': 'Basic ' + base64.b64encode((CLIENT_ID + ':' + CLIENT_SECRET).encode()).decode()
        }
        req_body = {
                'grant_type': 'refresh_token',
                'refresh_token': gl.auth_codes['refresh_token'],
                'client_id': CLIENT_ID,
                'client_secret': CLIENT_SECRET
            }
        response = requests.post(TOKEN_URL, headers=req_headers, data=req_body)
        new_token_info = response.json()
        if (response.status_code == 200):
            print('Access token accessed.')
            gl.auth_codes['access_token'] = new_token_info['access_token']
            gl.auth_codes['expires_at'] = datetime.now().timestamp() + new_token_info['expires_in']
            gl.def_header = {  # the default header
                'Authorization': f'Bearer {auth_codes["access_token"]}'
            }
            SAVE(gl.auth_codes, 'auth.obj') # saves the new auth codes

            # loads from pickled 
            gl.auth_codes = LOAD('auth.obj') 
            gl.def_header = {  # sets the default header
                        'Authorization': f'Bearer {gl.auth_codes["access_token"]}' 
            }
