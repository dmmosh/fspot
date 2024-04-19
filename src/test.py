from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from tkhtmlview import HTMLLabel  # For tkhtmlview
from PyQt5.QtCore import QUrl, QCoreApplication
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtWebEngineWidgets import QWebEngineView
# Or
# import webview  # For pywebview

# WEB PLAYBACK SDK

# creating main window class
class MainWindow(QMainWindow):
 
    # constructor
    def __init__(self, parent=None):
        super().__init__(parent)
        
        # creating a QWebEngineView
        self.browser = QWebEngineView()
        
        # setting default browser url as google
        self.browser.load(QUrl("file://" + FOLDER+'player.html?access_token=' + gl.auth_codes['access_token']))
        # set this browser as central widget or main window
        self.setCentralWidget(self.browser)
    

# starts the browser in a thread
def start_browser():
    # WEB PLAYER STARTUP
    app = QApplication([])
    app.setApplicationName("fspot player")
    
    window = MainWindow()
    window.show()
    return app.exec()
