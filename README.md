# PyEverything
 Python bindings for the Everything Service API
 
Everything is a windows-based program that lets a user index and very quickly search on the order of millions of files.
There is an SDK for the program, which is written in C.
 
PyEverything is exactly what the name suggests: Python bindings for the C functionality so that python scripters can leverage this extremely powerful service.
 
### Requirements
 1. Python 3.7 64-bit or later.
 2. Windows
 3. Everything Service 1.4.1+ installed https://www.voidtools.com/

### Installation 

    pip install PyEverything

If your version of python is not officially supported, but you still wish to use it, follow the "Building from Source" instructions.

## Usage
Basic API usages involves three main phases:
1. Set up the query search state
2. Submit the query
3. Iterate over the resulting data via index
### example

    import PyEverything

    PyEverything.SetSearch("example")
    PyEverything.Query()
    for i in PyEverything.GetNumResults():
        print(PyEverything.GetResultFullPathName(i))
        #results will get printed out here.

The full Everything SDK is here:   [https://www.voidtools.com/support/everything/sdk/](https://www.voidtools.com/support/everything/sdk/)
There are some key differences between the python SDK and the C one.

### Python Key Differences

#### Return Types
There are several functions that provide some data in the C API, and do so by requiring a pointer to a buffer that the API then fills.
For example, on [https://www.voidtools.com/support/everything/sdk/everything_getresultfullpathname/](https://www.voidtools.com/support/everything/sdk/everything_getresultfullpathname/)

    TCHAR buf[MAX_PATH];  
  
    // set the search text to abc AND 123  
    Everything_SetSearch("abc 123");  
  
    // execute the query  
    Everything_Query(TRUE);  
  
    // Get the full path and file name of the first visible result.  
    Everything_GetResultFullPathName(0,buf,sizeof(buf) / sizeof(TCHAR));
In PyEverything, this will be a returned string:

    #set the search text to abc AND 123  
    PyEverything.SetSearch("abc 123")
    
    #execute the query  
    PyEverything.Query(True)
    
    #Get the full path and file name of the first visible result.  
    result = PyEverything.GetResultFullPathName(0)

Functions that return a date will return `None` if that data could not be queried. (Which might be the case with incomplete indexes of drives that are no longer available.)

    GetResultDateAccessed()
    GetResultDateCreated()
    GetResultDateModified()
    GetResultDateRecentlyChanged()
    GetResultDateRun()

#### Error Handling
A large portion of the C API returns error codes, which the user must handle.
For example, on [https://www.voidtools.com/support/everything/sdk/everything_getlasterror/](https://www.voidtools.com/support/everything/sdk/everything_getlasterror/)

    // execute the query  
    if (!Everything_Query(true))  
    {  
	    DWORD dwLastError = Everything_GetLastError();  
	    if (dwLastError == EVERYTHING_ERROR_IPC)  
    	{  
		    // IPC not available.  
	    }  
    }
In Python, this is instead a `RuntimeError`:
    
    >>> import PyEverything
    >>> PyEverything.SetSearch("abc 123")
    >>> PyEverything.Query() #Raises PyEverything.ERROR_IPC
    Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
    RuntimeError: Everything API returned error code: 2
The numerical value of the exception is the same as on the above GetLastError page.

#### Namespaces
C lacks namespaces, so all Everything SDK methods and variables have the "Everything_" prefix.
the "PyEverything" package is already a namespace, so this prefix has been dropped everywhere that it appears. Keyboard switches are expensive, you shouldn't wear them out!

# Building from Source

Building from source is done via python and setuptools. The extension module is created with pybind11, a super easy to use library that lets users wrap complex C code and expose it in a .pyd file.
### dependencies

 - Pybind11 (included in repo) : https://github.com/pybind/pybind11 
 - Everything SDK (included in repo) : [https://www.voidtools.com/support/everything/sdk/](https://www.voidtools.com/support/everything/sdk/) 
 - Python 3.7 64 bit environment
 - wheel from pypi: `pip install wheel`
 - VS 2015 build tools or later (Dependent on Python version. For more information on that, [check out this page](https://wiki.python.org/moin/WindowsCompilers)

### Instructions
1. Clone this repo and initialise submodules:
    * `git clone --recurse-submodules https://github.com/nambread/PyEverything`
2. Activate your virtual environment of choice.
3. CD to the root repo folder and run `python setup.py bdist_wheel`
4. You should get a wheel for your python version put in the newly created .\dist folder.
5. `pip install path/to/wheel.whl`
