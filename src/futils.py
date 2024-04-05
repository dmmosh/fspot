from fglobal import *
import fglobal as gl


# HELPER FUNCTIONS

# saves the pickle object to var folder
def SAVE(to_save, var_name:str): # pickle saving variables
    pickle.dump(to_save, open(FOLDER+ 'var/' + var_name, 'wb'))

# loads the pickle object from the var folder
def LOAD(var_name:str): # pickle loading variables
    if os.path.exists(FOLDER+ 'var/' + var_name):
        return pickle.load(open(FOLDER+ 'var/' + var_name, 'rb')) 
    else:
        return None


# adds the authorization code to the header
def HEADER(request:dict = None)-> dict: # returns a dict
    return gl.def_header if request is None else request | gl.def_header 
    # returns just the default header if empty OR the request with the def header

def GET(where_from:str, request:dict = None): # a get request, retrieves resources
    return requests.get(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)

def PUT(where_from:str, request:dict = None): # a post request, creates resources
    return requests.put(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)

def POST(where_from:str, request:dict = None): # a put request, changes and/or replaces resources
    return requests.post(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)

def DELETE(where_from:str, request:dict = None): # a delete request, deletes resources
    return requests.delete(BASE_URL + where_from, headers=HEADER(request), allow_redirects=True)