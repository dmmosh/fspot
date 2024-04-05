from fglobal import *
import fglobal as gl


# HELPER FUNCTIONS

# saves the pickle object to var folder
def SAVE(to_save, var_name:str): # pickle saving variables
    pickle.dump(to_save, open(FOLDER+ 'var/' + var_name, 'wb'))

# loads the pickle object from the var folder
def LOAD(var_name:str): # pickle loading variables
    return pickle.load(open(FOLDER+ 'var/' + var_name, 'rb')) 


# adds the authorization code to the header
def HEADER(request:dict = None)-> dict: 
    return request | gl.def_header

def GET(where_from:str, request:dict = None)->dict:
    