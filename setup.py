from setuptools import setup, Extension, find_packages
import os

desc = '''
    Search Everything SDK, but in python!
    '''

pyd_ext = Extension('PyEverything._PyEverything', 
                    ['PyEverything/_PyEverything/PyEverything.cpp',
                     'PyEverything/_PyEverything/vendor/Everything-SDK/src/Everything.c'],
                    include_dirs=[
                        'pybind11/include', 
                        'PyEverything/_PyEverything/vendor/Everything-SDK/include',
                        'PyEverything/_PyEverything/vendor/Everything-SDK/ipc'
                        ],
                    define_macros=[('_CRT_SECURE_NO_WARNINGS', None),('UNICODE', None)],
                    library_dirs=[],
                    libraries=[])


setup(
        name='PyEverything',
        description='Search Everything SDK, but in python!',
        version = '0.2',
        author='Michael Nam',
        author_email='michael.nam@namination.co.uk',
        license='MIT',
        url='https://github.com/nambread/Everything_Python',
        packages=['PyEverything'],
        ext_modules = [pyd_ext],
        python_requires = '>=3.7,<3.10',
        platforms = ['windows']
    )