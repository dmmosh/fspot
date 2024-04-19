from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from tkhtmlview import HTMLLabel  # For tkhtmlview
# Or
# import webview  # For pywebview

class SimpleBrowser(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Simple Python Browser")

        # Create a web view component
        self.web_view = HTMLLabel(self, html="")
        self.web_view.pack(fill=tk.BOTH, expand=True)

        # Or, for pywebview:
        # self.web_view = webview.create_window("Simple Python Browser")

        # Load an initial URL
        self.load_url("https://google.com")

    def load_url(self, url):
        # Load a URL into the web view
        self.web_view.set_html(f'<iframe src="{url}" width="100%" height="100%"></iframe>')

        # Or, for pywebview:
        # self.web_view.load_url(url)

if __name__ == "__main__":
    # Create and run the browser window
    browser = SimpleBrowser()
    browser.load_url('google.com')
    browser.geometry("800x600")  # Set initial window size
    browser.mainloop()