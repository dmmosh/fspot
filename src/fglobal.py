import urllib.parse
import requests
import logging
import click
import os
import webbrowser
from datetime import datetime, timedelta
from flask import Flask, redirect, request, jsonify, session
import urllib
import pickle

auth_codes = { # authorization codes
    'access_token': '',
    'refresh_token': '',
    'expires_at': ''
}

def_header = {  # the default header
        'Authorization': f'Bearer {auth_codes["access_token"]}'
}

auth_str = f'Bearer {auth_codes["access_token"]}' # the authorization string (for custom headers)

FOLDER = 'fspot/' # folder

CLIENT_ID = 'bbdff8f6b6524edc90d968c3f971b5da'
CLIENT_SECRET = "" # change in fmain
REDIRECT_URI = 'http://localhost:5000/callback'

AUTH_URL = 'https://accounts.spotify.com/authorize'
TOKEN_URL = 'https://accounts.spotify.com/api/token'
BASE_URL = 'https://api.spotify.com/v1/'