import urllib.parse
import requests
import logging
import click
import os
import time
import webbrowser
import pyAesCrypt
from datetime import datetime, timedelta
from flask import Flask, redirect, request, jsonify, session
import urllib
import pickle

'''
GLOBAL VARIABLES AND IMPORTS

'''

auth_codes = {'access_token': '', 'refresh_token': '', 'expires_at': 0} # the auth code default values, will be overriden

def_header = {} # the default header (changes to authorization )

FOLDER = 'fspot/' # folder

CLIENT_ID = 'bbdff8f6b6524edc90d968c3f971b5da' # the client id
CLIENT_SECRET = '' # client secret
PASSWORD = '' # pyaescrypt password
REDIRECT_URI = 'http://localhost:5000/callback' # the redirect uri

AUTH_URL = 'https://accounts.spotify.com/authorize' # the authorization url
TOKEN_URL = 'https://accounts.spotify.com/api/token' # the token url
BASE_URL = 'https://api.spotify.com/v1/' # the base url