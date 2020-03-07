
import os
import sys

pyd_path_debug = r'Everything-Python\build\x64\Debug'
pyd_path_resolved = os.path.join(os.path.dirname(__file__), '..', pyd_path_debug)
sys.path.append(pyd_path_resolved)

print(f"Added to path {pyd_path_resolved}")

import Everything_Python

testSearch = Everything_Python.TestSearch("system32")
print(testSearch)
'''break'''