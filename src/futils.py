from fglobal import *
import fglobal as gl

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

# centers the text, normal center doesnt work because it fills the right side with padding too
def CENTER(input:str)-> str:
    return (' '* ((gl.term_size-len(input))//2))+ input

# a loading message, clears itself when finishes
def loading_msg(process:threading.Thread, msg:str = 'Loading...')-> None:
    title = {'id': 1,   # the title slide properties
             'line_num': 3,
             'col_num': 10,
             }

    # choose which title number to print (depending on column terminal size
    if gl.term_size < 42: # TITLE 1 (under 42)
        title = {'id': 1, 'line_num': 3, 'col_num': 10}

    elif gl.term_size < 52: # TITLE 2 (42 and over)
        title = {'id': 2, 'line_num': 9, 'col_num': 42}

    elif gl.term_size < 123: # TITLE 3 (52 and over)
        title = {'id': 3, 'line_num': 10, 'col_num': 52}

    else: # TITLE 4 (123 and over)
        title = {'id': 4, 'line_num': 23, 'col_num': 123}

    
    title_text = open(FOLDER + 'titles/title' + str(title['id']) + '.txt', 'r')
    print(TEXT['bold_on'] + title_text.read() + TEXT['bold_off'])

    title_text.close()


    print_msg = msg
    while(process.is_alive()):
        for char in "/—\|":
            if gl.term_size < title['col_num']:

                title_text = open(FOLDER + 'titles/title1.txt', 'r')
                
                print(title_text.read())
                title_text.close()
                title = {'id': 1, 'line_num': 3, 'col_num': 10, 'len': 11}


            # everything message related
            print_msg = msg[:gl.term_size-7] + '...  ' if (len(msg)+1 > gl.term_size) else msg # shortens the print message if need be
            sys.stdout.write( TEXT['invert_on'] + print_msg + char + '\r' + TEXT['invert_off'] )
            sys.stdout.flush()
            time.sleep(0.15)

    clear_line()
    while(title['line_num']): # clear the title screen
        move_up()
        clear_line()
        title['line_num']-=1
    
    

    #print("\r") # carriage return
    

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



# DELETES LAST LINE
def clear_line(replace_with:str = '')-> None:
    print('\x1b[2K' + replace_with, end='')

def move_up(n:int = 1)-> None:
    while(n):
        sys.stdout.write('\x1b[1A')
        n-=1


# cool clear function
def clear():
    lines = os.get_terminal_size().lines
    print('\x1b[100E', end='')
    time.sleep(1)
    #print( TEXT['line_on'] + ' '+  TEXT['line_off'] ,end='')
    while(lines):
        move_up()
        print('\x1b[1F' + TEXT['line_on'] + ' '+  TEXT['line_off'] ,end='')
        time.sleep(0.22)
        lines -=1
    time.sleep(1)
    os.system('clear') # tidies everything up

# REFRESH THE ACCESS TOKEN
def refresh(force:bool = False):
    if (datetime.now().timestamp() > gl.auth_codes['expires_at']) or force:
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
