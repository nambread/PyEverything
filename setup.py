from setuptools import setup, Extension, find_packages
import os
import shutil

desc = '''
    
    '''

pyd_ext = Extension('Everything_Python._Everything_Python', 
                    ['Everything_Python/_Everything_Python/Everything-Python.cpp'],
                    include_dirs=['pybind11/include', 'Everything_Python/_Everything_Python/vendor/Everything-SDK/include'],
                    define_macros=[('_CRT_SECURE_NO_WARNINGS', None),('UNICODE', None)],
                    library_dirs=['Everything_Python/_Everything_Python/vendor/Everything-SDK/lib'],
                    libraries=['Everything64'])

#copy the dll to the pyd location, due to changes to how .pyd dependencies are loaded: https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
try:
    shutil.copyfile('Everything_Python/_Everything_Python/vendor/Everything-SDK/dll/Everything64.dll', 'Everything_Python/Everything64.dll')
    setup(
            name='Everything_Python',
            description='Search Everything SDK, but in python!',
            version = '0.1',
            author='Michael Nam',
            author_email='michael.nam@namination.co.uk',
            license='MIT',
            url='',
            package_data={'Everything_Python' : ['Everything64.dll']},
            packages=['Everything_Python'],
            ext_modules = [pyd_ext]
        )
finally:
    os.remove('Everything_Python/Everything64.dll')