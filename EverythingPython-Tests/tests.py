import unittest
import Everything_Python as EP

class TestSearchStateMethods(unittest.TestCase):

    def test_request_flags(self):
        search_flags = EP.REQUEST_SIZE | EP.REQUEST_FILE_NAME | EP.REQUEST_PATH | EP.REQUEST_DATE_CREATED
        EP.SetRequestFlags(search_flags)
        set_search_flags = EP.GetRequestFlags()
        assert(search_flags & set_search_flags == search_flags)

if __name__ == "__main__":
    search_flags = EP.REQUEST_SIZE | EP.REQUEST_FILE_NAME | EP.REQUEST_PATH | EP.REQUEST_DATE_CREATED
    EP.SetRequestFlags(search_flags)
    EP.SetSearch("test")
    EP.Query()
    print(EP.GetResultDateCreated(0))
    '''break'''