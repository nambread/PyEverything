rmdir /Q /S .\build
rmdir /Q /S .\dist
call env37\scripts\activate
python setup.py bdist_wheel
call env38\scripts\activate
python setup.py bdist_wheel
call env39\scripts\activate
python setup.py bdist_wheel