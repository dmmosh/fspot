from fglobal import *
from futils import *
import fglobal as gl

app = Flask(__name__)
app.secret_key = open("src/key.txt", "r").readline()
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

def secho(text, file=None, nl=None, err=None, color=None, **styles):
    pass

def echo(text, file=None, nl=None, err=None, color=None, **styles):
    pass

click.echo = echo
click.secho = secho

CLIENT_SECRET = open("src/secret.txt", "r").readline()



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
        return jsonify({'error': request.args['error']}) 
    
    if 'code' in request.args: # if login is successful
        req_body = { 
            'code': request.args['code'], # the code
            'grant_type': 'authorization_code', # the auth code
            'redirect_uri': REDIRECT_URI, # redirect uri
            'client_id': CLIENT_ID, # client id
            'client_secret': CLIENT_SECRET # client secret
        }

    response = requests.post(TOKEN_URL, data=req_body)
    token_info = response.json()

    gl.auth_codes['access_token'] = token_info['access_token'] # the access token, lasts a day
    gl.auth_codes['refresh_token'] = token_info['refresh_token'] # the refresh token
    gl.auth_codes['expires_at'] = datetime.now().timestamp() + token_info['expires_in'] # duration the access token lasts

    gl.def_header = {  # the default header
            'Authorization': f'Bearer {auth_codes["access_token"]}'
    }

    return redirect('/playlists')


@app.route('/playlists')
def get_playlists():
    if 'access_token' not in gl.auth_codes: # if no access token
        return redirect('/login')
    
    if datetime.now().timestamp() > gl.auth_codes['expires_at']:
        return redirect('/refresh-token')

    headers = {
        'Authorization': f'Bearer {gl.auth_codes["access_token"]}'
    }

    name = GET('me')

    if(name.status_code == 200):
        return f'Successfully logged in, {name.json()["display_name"]}!<br>You can now quit this page.'
    else:
        return redirect('/')


@app.route('/refresh_token')
def refresh_token():
    if 'refresh_token' not in gl.auth_codes:
        return redirect('/login')
    
    if datetime.now().timestamp() > gl.auth_codes['expires_at']:
        req_body = {
            'grant_type': 'refresh_token',
            'refresh_token': gl.auth_codes['refresh_token',],
            'client_id': CLIENT_ID,
            'client_secret': CLIENT_SECRET
        }

        response = requests.post(TOKEN_URL, data=req_body)
        new_token_info = response.json()
        gl.auth_codes['access_token'] = new_token_info['access_token']
        gl.auth_codes['expires_at'] = datetime.now().timestamp() + new_token_info['expires_in']

        return redirect('/playlists')

gl.auth_codes = LOAD('auth.obj')
if gl.auth_codes is None or GET('me') != 200:
    webbrowser.open('http://127.0.0.1:5000')
    app.run(debug=False)
    SAVE(gl.auth_codes, 'auth.obj')
    SAVE(gl.def_header, 'header.obj')

gl.auth_codes = LOAD('auth.obj')
gl.def_header = LOAD('header.obj')


data = GET('me')

    

print(data.json())