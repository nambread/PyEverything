# Everything_Python
 Python bindings for the Everything Service API
 
Everything is a windows-based program that lets a user index and very quickly search on the order of millions of files.
There is an SDK for the program, which is written in C.
 
Everything-Python is exactly what the name suggests: Python bindings for the C functionality so that python scripters can leverage this extremely powerful service.
 
### Requirements
 1. Python 3.7 64-bit or later.
 2. Everything Service installed https://www.voidtools.com/

### Installation 
Download the latest revision from the releases page for your version of python
Run

    pip install path-to-the-.whl

## Usage
Basic API usages involves three main phases:
1. Set up the query search state
2. Submit the query
3. Iterate over the resulting data
### example

    import Everything_Python

    Everything_Python.SetSearch("example")
    Everything_Python.Query()
    for i in Everything_Python.GetNumResults():
        print(Everything_Python.GetResultFullPathName(i))
        #results will get printed out here.

The full Everything SDK is here:   [https://www.voidtools.com/support/everything/sdk/](https://www.voidtools.com/support/everything/sdk/)
It is mostly a 1:1 translation, but there are some key differences between the python SDK and the C one.

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
In Everything_Python, this will simply be a returned piece of data:

    #set the search text to abc AND 123  
    Everything_Python.SetSearch("abc 123")
    
    #execute the query  
    Everything_Python.Query(True)
    
    #Get the full path and file name of the first visible result.  
    result = Everything_Python.GetResultFullPathName(0)

#### Error Handling
A large portion of the C API returns error codes, which the user must handle
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
In Python, this is instead a **RuntimeError**:
    
    >>> import Everything_Python
    >>> Everything_Python.SetSearch("abc 123")
    >>> Everything_Python.Query() #Raises Everything_Python.ERROR_IPC
    Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
    RuntimeError: Everything API returned error code: 2
The numerical value of the exception is the same as on the above GetLastError page.

#### Namespaces
C lacks namespaces, so all Everything SDK methods and variables have the "Everything_" prefix.
the "Everything_Python" package is already a namespace, so this prefix has been dropped everywhere that it appears. Keyboard switches are expensive, you shouldn't wear them out!

# Building from Source

Building from source is done via python and setuptools. The extension module is created with pybind11, a super easy to use library that lets user wrap complex C code and expose it in a .pyd file.
### dependencies

 - Pybind11 : https://github.com/pybind/pybind11
 - Everything SDK : [https://www.voidtools.com/support/everything/sdk/](https://www.voidtools.com/support/everything/sdk/)
 - Python 3.7 64 bit environment
 - VS 2015 build tools or later (Dependent on Python version)

### Instructions
1. Download this repo somewhere local
2. Download the pybind11 repo into the root folder of this one, so that the path .\pybind11\include is valid and points to the header files.
3. Download and extract the Everything SDK to the root folder of this repo, to a folder called "EverythingSDK" such that .\EverythingSDK\include points to the header files, and .\EverythingSDK\lib points to the libs
4. Copy .\EverythingSDK\dll\Everything64.dll to the .\Everything_Python\Everything_Python folder
5. CD to the root repo folder and run **python setup.py bdist_wheel**
6. You should get a wheel of your python version put in the newly created .\dist folder.
