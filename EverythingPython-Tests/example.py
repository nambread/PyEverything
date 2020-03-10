
import os
import sys

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
