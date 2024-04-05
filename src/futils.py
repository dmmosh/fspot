from fglobal import *
import fglobal as gl


# HELPER FUNCTIONS

# saves the pickle object to var folder
def SAVE(to_save, var_name:str): # pickle saving variables
    pickle.dump(to_save, open(FOLDER+ 'var/' + var_name, 'wb'))
    pyAesCrypt.encryptFile(FOLDER+'var/'+var_name, FOLDER+'var/'+var_name + '.aes', PASSWORD)
    os.remove(FOLDER+ 'var/' + var_name)

# loads the pickle object from the var folder
def LOAD(var_name:str): # pickle loading variables
    if os.path.exists(FOLDER+ 'var/' + var_name + '.aes'):
        pyAesCrypt.decryptFile(FOLDER+'var/'+var_name+'.aes', FOLDER+'var/'+var_name, PASSWORD)
        output = pickle.load(open(FOLDER+ 'var/' + var_name, 'rb')) 
        os.remove(FOLDER+ 'var/' + var_name)
        return output
    else:
        return None

    
def ERROR(*args:str)->None: # prints error message
    print("FATAL ERROR:") 
    for to_print in args:
        print('\t' + to_print)

# adds the authorization code to the header
def HEADER(request:dict = None)-> dict: # returns a dict
    return gl.def_header if request is None else request | gl.def_header 
    # returns just the default header if empty OR the request with the def header

def GET(where_from:str, request:dict = None): # a get request, retrieves resources
    return requests.get(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)

def PUT(where_from:str, request:dict = None): # a post request, creates resources
    return requests.put(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)

def POST(where_from:str, request:dict = None): # a put request, changes and/or replaces resources
    return requests.post(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)

def DELETE(where_from:str, request:dict = None): # a delete request, deletes resources
    return requests.delete(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)

def refresh():
    if datetime.now().timestamp() > gl.auth_codes['expires_at']:
        req_body = {
                'grant_type': 'refresh_token',
                'refresh_token': gl.auth_codes['refresh_token'],
                'client_id': CLIENT_ID,
                'client_secret': CLIENT_SECRET
            }
        response = requests.post(TOKEN_URL, data=req_body)
        new_token_info = response.json()
        gl.auth_codes['access_token'] = new_token_info['access_token']
        gl.auth_codes['expires_at'] = datetime.now().timestamp() + new_token_info['expires_in']
        gl.def_header = {  # the default header
            'Authorization': f'Bearer {auth_codes["access_token"]}'
        }
        SAVE(gl.auth_codes, 'auth.obj') # saves the new auth codes
        SAVE(gl.def_header, 'header.obj') # saves the new header