import requests
from flask import Flask

app = Flask(__name__)
app.secret_key = 'dsklfji4o489yne4382n2fn8nv44854'


CLIENT_ID = 'bbdff8f6b6524edc90d968c3f971b5da'
CLIENT_SECRET = open("./client_secret.txt", "r").readline() # DEBUG PURPOSES
REDIRECT_URI = 'http://localhost:5000/callback'


