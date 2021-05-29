from setuptools import setup, Extension, find_packages
import os
import shutil

desc = '''
    Search Everything SDK, but in python!
    '''

pyd_ext = Extension('Everything_Python._Everything_Python', 
                    ['Everything_Python/_Everything_Python/Everything_Python.cpp',
                     'Everything_Python/_Everything_Python/vendor/Everything-SDK/src/Everything.c'],
                    include_dirs=[
                        'pybind11/include', 
                        'Everything_Python/_Everything_Python/vendor/Everything-SDK/include',
                        'Everything_Python/_Everything_Python/vendor/Everything-SDK/ipc'
                        ],
                    define_macros=[('_CRT_SECURE_NO_WARNINGS', None),('UNICODE', None)],
                    library_dirs=[],
                    libraries=[])


setup(
        name='Everything_Python',
        description='Search Everything SDK, but in python!',
        version = '0.2',
        author='Michael Nam',
        author_email='michael.nam@namination.co.uk',
        license='MIT',
        url='https://github.com/nambread/Everything_Python',
        packages=['Everything_Python'],
        ext_modules = [pyd_ext]
    )