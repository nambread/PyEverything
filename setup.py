from setuptools import setup, Extension, find_packages

desc = '''
    
    '''

pyd_ext = Extension('Everything_Python._Everything_Python', 
                    ['_Everything_Python/Everything-Python.cpp'],
                    include_dirs=['pybind11/include', 'EverythingSDK/include'],
                    define_macros=[('_CRT_SECURE_NO_WARNINGS', None),('UNICODE', None)],
                    library_dirs=['EverythingSDK/lib'],
                    libraries=['Everything64'])

setup(
        name='Everything_Python',
        description='Search Everything SDK, but in python!',
        version = '0.1',
        author='Michael Nam',
        author_email='michael.nam@namination.co.uk',
        license='MIT',
        url='',
        package_data={'Everything_Python': ['./Everything64.dll']},
        packages=['Everything_Python'],
        ext_modules = [pyd_ext]
    )