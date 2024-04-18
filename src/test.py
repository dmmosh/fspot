from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl

from PyQt5.QtCore import QUrl
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5 import QtCore, QtGui, QtWidgets
 
# creating main window class
class MainWindow(QMainWindow):
 
    # constructor
    def __init__(self, parent=None):
        super().__init__(parent)

        # creating a QWebEngineView
        self.browser = QWebEngineView()
            
        # setting default browser url as google
        self.browser.setUrl(QUrl("file://" + FOLDER + "player.html"))
 
        # set this browser as central widget or main window
        self.setCentralWidget(self.browser)
        # showing all the components
        self.show()


 
 
# creating a pyQt5 application
app = QApplication([])
# setting name to the application
app.setApplicationName("fspot player")
 
# creating a main window object
window = MainWindow()

 
# loop

sys.exit(app.exec())
#os.system('kill -9 ' + app.applicationPid())