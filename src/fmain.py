from fglobal import *
import fglobal as gl


"""
FSPOT

a fast spotify player

manual compiling:
python -m PyInstaller --onefile src/auth.py --name fspot
note: CHANGE THE SECRET KEY AND CLIENT SECRET AND THEN COMPILE
"""

app = Flask(__name__) # inits flask for auth
app.secret_key = open("src/key.txt", "r").readline() #CHANGE FOPR COMPILING


# TO SILENCE FLASK MESSAGE
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
    scope = 'playlist-read-private user-library-modify user-library-read user-read-playback-state user-modify-playback-state user-read-currently-playing app-remote-control streaming'
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

    session['access_token'] = token_info['access_token'] # the access token, lasts a day
    session['refresh_token'] = token_info['refresh_token'] # the refresh token
    session['expires_at'] = datetime.now().timestamp() + token_info['expires_in'] # duration the access token lasts

    return redirect('/playlists')


@app.route('/playlists')
def get_playlists():
    if 'access_token' not in session: # if no access token
        return redirect('/login')
    
    if datetime.now().timestamp() > session['expires_at']: # if session expired
        return redirect('/refresh-token')

    headers = {
        'Authorization': f'Bearer {session["access_token"]}'
    }

    response = requests.get(API_BASE_URL + 'me/playlists', headers=headers)
    playlists = response.json()
    return jsonify(playlists)


@app.route('/refresh_token')
def refresh_token():
    if 'refresh_token' not in session:
        return redirect('/login')
    
    if datetime.now().timestamp() > session['expires_at']:
        req_body = {
            'grant_type': 'refresh_token',
            'refresh_token': session['refresh_token',],
            'client_id': CLIENT_ID,
            'client_secret': CLIENT_SECRET
        }

        response = requests.post(TOKEN_URL, data=req_body)
        new_token_info = response.json()
        session['access_token'] = new_token_info['access_token']
        session['expires_at'] = datetime.now().timestamp() + new_token_info['expires_in']

        return redirect('/playlists')
    

webbrowser.open('http://127.0.0.1:5000')
app.run(debug=False)

