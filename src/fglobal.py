import urllib.parse
import requests
import logging
import click
import webbrowser
from datetime import datetime, timedelta
from flask import Flask, redirect, request, jsonify, session
import urllib

CLIENT_ID = 'bbdff8f6b6524edc90d968c3f971b5da'
CLIENT_SECRET = open("src/secret.txt", "r").readline()
REDIRECT_URI = 'http://localhost:5000/callback'

AUTH_URL = 'https://accounts.spotify.com/authorize'
TOKEN_URL = 'https://accounts.spotify.com/api/token'
API_BASE_URL = 'https://api.spotify.com/v1/'