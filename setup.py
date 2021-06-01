from setuptools import setup, Extension, find_packages
import os
import pathlib

desc = '''
    Search Everything SDK, but in python!
    '''
here = os.path.abspath(os.path.dirname(__file__))

about = {}
NAME = 'PyEverything'
with open(os.path.join(here, NAME, '__version__.py')) as f:
    exec(f.read(), about)
VERSION = about['__version__']

# The text of the README file
README = (pathlib.Path(here) / "README.md").read_text()

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
        name=NAME,
        description='Search Everything SDK, but in python.',
        version = VERSION,
        author='Michael Nam',
        author_email='michael.nam@namination.co.uk',
        license='MIT',
        url='https://github.com/nambread/PyEverything',
        packages=[NAME],
        ext_modules = [pyd_ext],
        include_package_data=True,
        package_data={NAME: ['_PyEverything.pyi']},
        python_requires = '>=3.7,<3.10',
        long_description=README,
        long_description_content_type="text/markdown",
        platforms = ['windows'],
        classifiers=[
            'License :: OSI Approved :: MIT License',
            'Programming Language :: Python',
            'Programming Language :: Python :: 3 :: Only',
            'Programming Language :: Python :: 3.7',
            'Programming Language :: Python :: 3.8',
            'Programming Language :: Python :: 3.9',
            'Programming Language :: Python :: Implementation :: CPython',
            
        ]
    )