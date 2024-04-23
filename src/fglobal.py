from datetime import datetime, timedelta
from flask import Flask, redirect, request, jsonify, session
from getpass import getpass
from random import randint
import termios
import fcntl
import keyboard as kb
import requests
import atexit
import os
import time
import pyAesCrypt
import sys
import threading
from multiprocessing import Process
import subprocess
import math
import signal 
import webbrowser
import urllib
import io
import base64
import pickle
import json

'''
GLOBAL VARIABLES AND IMPORTS

'''

term_size = 0



auth_codes = {'access_token': '', 
              'refresh_token': '', 
              'user_id':'', 
              'password': '', 
              'expires_at': -1} # the auth code default values, will be overriden

def_header = {  # sets the default header
            'Authorization': f'Bearer {auth_codes["access_token"]}' 
    }

FOLDER = os.environ['HOME'] + '/coding/fspot/fspot/' # folder, change later

DIRNAME, FILENAME = os.path.split(os.path.abspath(__file__)) # fglobal directory

# TODO: change these when compiling
SECRET_KEY = os.environ['FSPOT_FLASK_KEY']
CLIENT_SECRET = os.environ['FSPOT_CLIENT_SECRET']
PASSWORD = os.environ['FSPOT_PASSWORD']
# FOLDER = '/usr/share/fspot/' # for distribution

CLIENT_ID = 'bbdff8f6b6524edc90d968c3f971b5da' # the client id
REDIRECT_URI = 'http://localhost:5000/callback' # the redirect uri

AUTH_URL = 'https://accounts.spotify.com/authorize' # the authorization url
TOKEN_URL = 'https://accounts.spotify.com/api/token' # the token url
BASE_URL = 'https://api.spotify.com/v1/' # the base url