
import os
import sys

project_root = os.path.normpath(os.path.join(os.path.dirname(__file__), '..'))

pyd_path = r'Everything-Python\build\x64\Debug'
pyd_path_resolved = os.path.join(project_root, pyd_path)
dll_path = r'..\venv\Scripts'
dll_path_resolved = os.path.join(project_root, dll_path)
sys.path.append(pyd_path_resolved)
sys.path.append(dll_path_resolved)

print(f"Added to path {pyd_path_resolved}")

import Everything_Python
from Everything_Python import Query, SetSearch, GetNumResults, GetResultFullPathName

MAX_RESULTS = 30

if __name__ == "__main__":
    while True:
        try:
            search = input("Enter your search term: ")
            if not search:
                search = "\\"
            SetSearch(search)
            Query(wait=True)
            numResults = GetNumResults()
            maxResults = min(numResults, MAX_RESULTS)
            results = [GetResultFullPathName(i, 260) for i in range(maxResults)]
            print("Printing the first 30 results:")
            results = '\n'.join(results)
            print(f"{results}")
        except BaseException as e:
            print(e)
