from fglobal import *
from futils import *
import fglobal as gl 
import logging
import click

app = Flask(__name__)
app.secret_key = gl.SECRET_KEY
exiting = False # to exit the program


browser_run = Process(target=app.run)

def login_start():
    global browser_run
    browser_run.start()
    webbrowser.open('http://127.0.0.1:5000')

    browser_run.join()
    browser_run.terminate()

    # loads from pickled 
    gl.auth_codes = LOAD('auth.obj') 
    gl.def_header = {  # sets the default header
            'Authorization': f'Bearer {gl.auth_codes["access_token"]}' 
    }

    get_password() # gets the user's password

    
# GETS USER'S PASSWORD 
# only run after login
def get_password():

    user = GET('me') # gets user info
    if (user.status_code != 200): # base case
        ERROR('Unable to connect to the API.')

    user = user.json()

    msg = 'Hello ' + user['display_name'] + ', please enter your password:  '

    input = 
    (msg) # gets the password
    if input == '':
        clear_string(len(msg)) # clears the password box
        get_password()
        return
    
    gl.auth_codes['user_id'] = user['id']
    gl.auth_codes['password'] = input
    SAVE(gl.auth_codes, 'auth.obj') # saves the password

    # loads from pickled 
    gl.auth_codes = LOAD('auth.obj') 
    gl.def_header = {  # sets the default header
            'Authorization': f'Bearer {gl.auth_codes["access_token"]}' 
    }

    clear_string(len(msg)) # clears the password box




# suppress the flask output
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
def secho(text, file=None, nl=None, err=None, color=None, **styles):
    pass
def echo(text, file=None, nl=None, err=None, color=None, **styles):
    pass
click.echo = echo
click.secho = secho



# redirect to login screen
@app.route('/')
def login():
    scope = '''app-remote-control 
                streaming 
                user-top-read 
                user-read-currently-playing 
                user-read-recently-played 
                user-read-playback-state 
                user-modify-playback-state 
                user-read-private 
                user-read-email'''
    params = {
        'client_id': CLIENT_ID, # the client id
        'response_type': 'code', # the response data type
        'scope': scope,  # the scope 
        'redirect_uri': REDIRECT_URI, # redirect uri
        'show_dialog': True # saving the access token, for debug
    }

    auth_url = f'{AUTH_URL}?{urllib.parse.urlencode(params)}'

    return redirect(auth_url)

# callback, what spotify returns once login
@app.route('/callback')
def callback():
    if 'error' in request.args: # if login is unsuccessful
        if request.args['error'] == 'access_denied': # if user denied access
            ERROR('You denied the authorization request.')
        return '''<title>FATAL ERROR:</title><br>   
                    You denied the authorization request.<br>   
                    <a href="/"><button>Try again?</button>''' # returns html to callback page
    
    if 'code' in request.args: # if login is successful
        req_body = { 
            'code': request.args['code'], # the code
            'grant_type': 'authorization_code', # the auth code
            'redirect_uri': REDIRECT_URI, # redirect uri
            'client_id': CLIENT_ID, # client id
            'client_secret': CLIENT_SECRET # client secret
        }

    response = requests.post(TOKEN_URL, data=req_body) # the response
    if response.status_code != 200: # if the response didnt come back for whatever reason
        ERROR('Something went wrong. Unable to get authorization credentials.') # error throw
        return '''<title>FATAL ERROR:</title><br>   
                    Something went wrong. 
                    Unable to get authorization credentials.<br>   
                    <a href="/"><button>Try again?</button>''' # returns html to callback page


    token_info = response.json() # the token info

    gl.auth_codes['access_token'] = token_info['access_token'] # the access token, lasts a day
    gl.auth_codes['refresh_token'] = token_info['refresh_token'] # the refresh token
    gl.auth_codes['expires_at'] = datetime.now().timestamp() + token_info['expires_in'] # duration the access token lasts

    gl.def_header = {  # sets the default header
            'Authorization': f'Bearer {auth_codes["access_token"]}' 
    }
    SAVE(gl.auth_codes, 'auth.obj')


    return redirect('/success_login') # redirects to the success login page


@app.route('/success_login')
def success_login():
    name = GET('me')

    if(name.status_code == 200):
        global exiting
        exiting = True
        return "SUCCESS"
    else:
        ERROR('Something went wrong. Please try again.')
        return redirect('/')

#return "<script>window.open('','_self').close()</script>"

@app.teardown_request
def teardown(exception):
    global browser_run
    if exiting:
        return os._exit(0)



    

