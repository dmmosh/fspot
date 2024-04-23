from fglobal import *
from futils import *
from flogin import *
import fglobal as gl
import flogin as fl
import tkinter as tk
from pynput.keyboard import Key, Listener

# a loading message, clears itself when finishes
def loading_msg(process:threading.Thread, msg:str = 'Loading...')-> None:

    print_msg = msg
    while(process.is_alive()):

        term_col = os.get_terminal_size().columns
        for char in "/—\|":
            term_col = os.get_terminal_size().columns # gets column size of curr iteration

            title_num = 1 # title print number (stored in the titles folder)
            # choose which title number to print (depending on column terminal size)
            
            
            if term_col < 52: # if undex 52 columns (title 3 width)
                title_num = 2
            
            if title_col < 123:
                title_num = 4
            else:
                title_num = 5

            title_text = open(FOLDER + 'titles/title' + title_num + '.txt', 'r')
            print(title_text.read())
            title_text.close()


            # everything message related
            print_msg = msg[:term_col-7] + '...  ' if (len(msg)+1 > term_col) else msg # shortens the print message if need be
            sys.stdout.write(print_msg + char + '\r')
            sys.stdout.flush()
            time.sleep(0.15)
    clear_string(len(print_msg))
