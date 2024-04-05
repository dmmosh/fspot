from fglobal import *
from futils import *
import fglobal as gl


'''
FSPOT
a lightweight spotify clients

manual compiling:
python -m PyInstaller --onefile src/fmain.py --name fspot

remove the pickled files:
rm ./fspot/var/*
'''

app = Flask(__name__)

# suppress the flask output
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
def secho(text, file=None, nl=None, err=None, color=None, **styles):
    pass
def echo(text, file=None, nl=None, err=None, color=None, **styles):
    pass
click.echo = echo
click.secho = secho


# TODO: change these when compiling
app.secret_key = os.environ['FSPOT_FLASK_KEY']
CLIENT_SECRET = os.environ['FSPOT_CLIENT_SECRET']
gl.PASSWORD = os.environ['FSPOT_PASSWORD']
# gl.FOLDER = '/usr/share/fspot/' # for distribution



# redirect to login screen
@app.route('/')
def login():
    scope = 'user-read-private user-read-email'
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
        return '''FATAL ERROR:<br>   
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
        return '''FATAL ERROR:<br>   
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
    SAVE(gl.def_header, 'header.obj')

    return redirect('/success_login') # redirects to the success login page


@app.route('/success_login')
def success_login():
    if 'access_token' not in gl.auth_codes: # if no access token
        return redirect('/login')
    
    if datetime.now().timestamp() > gl.auth_codes['expires_at']:
        return redirect('/refresh-token')


    name = GET('me')

    if(name.status_code == 200):
        return f'Successfully logged in, {name.json()["display_name"]}!<br>You can now quit this page.'
    else:
        print("SOMETHING WENT WRONG. UNSUCCESSFUL LOGIN. TRY AGAIN.")
        return redirect('/')
    

@app.route('/refresh_token')
def refresh_token():
    if 'refresh_token' not in gl.auth_codes:
        return redirect('/login')    
    refresh()
    return redirect('/success_login')


# BASICALLY MAIN

load_var = LOAD('auth.obj') # loads the authorization info
if load_var is None: # if there isnt any, open sign in screen
    webbrowser.open('http://127.0.0.1:5000') 
    app.run(debug=False) # will save the auth codes

# loads from pickled 
gl.auth_codes = LOAD('auth.obj') 
gl.def_header = LOAD('header.obj')

refresh()
data = GET('me')

if data.status_code != 200:
    webbrowser.open('http://127.0.0.1:5000')
    app.run(debug=False)
    data = GET('me')

print(data.json())
    
