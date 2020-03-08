// Everything-Python.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <locale>
#include <codecvt>

#include "Everything.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;

/*
* Base exception for the everything API
* Future plan is to break out each error code into its own exception so that python API users can catch specific errors that they care about
* (e.g. catching the invalid ipc error to then boot the everything service.)
*/
struct EverythingException : std::exception 
{

	const DWORD m_errorCode;
	EverythingException(const DWORD& errorCode) : m_errorCode(errorCode) {}

	std::string errCodeString = std::to_string(this->m_errorCode);
	const std::string m_errorMessage = "Everything API returned error code: " + errCodeString;
	const char* what() const noexcept { return m_errorMessage.c_str(); }
};

/*
Converts the error codes into exceptions that get thrown.
@param status The status returned from various api calls. Most will return a 0 if the function fails.
Gets a bit confusing in cases where the return value is also used as a count, where 0 is a valid value.
*/
template<typename T>
static inline void checkEverythingErrorCodeAndThrow(const T status) 
{
	if (status == 0) 
	{
		DWORD errorCode = Everything_GetLastError();
		if (errorCode) 
		{
			//sometimes 0 is a valid status e.g when returning number of result matches.
			throw EverythingException(errorCode);
		}
	}
}

std::wstring Py_Test_Everything_Search(const std::wstring& searchString) 
{
	Everything_SetSearch(searchString.c_str());
	int ok = Everything_Query(true);
	checkEverythingErrorCodeAndThrow(ok);
	if (Everything_GetNumResults() > 0) 
	{
		TCHAR buf[MAX_PATH];
		int ok = Everything_GetResultFullPathName(0, buf, sizeof(buf) / sizeof(TCHAR));
		checkEverythingErrorCodeAndThrow(ok);
		const std::wstring result = std::wstring(buf);
		return result;
	}
	else 
	{
		return L"";
	}
}

/*
EVERYTHINGUSERAPI BOOL EVERYTHINGAPI Everything_GetMatchPath(void);
EVERYTHINGUSERAPI BOOL EVERYTHINGAPI Everything_GetMatchCase(void);
EVERYTHINGUSERAPI BOOL EVERYTHINGAPI Everything_GetMatchWholeWord(void);
EVERYTHINGUSERAPI BOOL EVERYTHINGAPI Everything_GetRegex(void);
EVERYTHINGUSERAPI DWORD EVERYTHINGAPI Everything_GetMax(void);
EVERYTHINGUSERAPI DWORD EVERYTHINGAPI Everything_GetOffset(void);
EVERYTHINGUSERAPI LPCSTR EVERYTHINGAPI Everything_GetSearchA(void);
EVERYTHINGUSERAPI LPCWSTR EVERYTHINGAPI Everything_GetSearchW(void);
EVERYTHINGUSERAPI DWORD EVERYTHINGAPI Everything_GetLastError(void);
EVERYTHINGUSERAPI HWND EVERYTHINGAPI Everything_GetReplyWindow(void);
EVERYTHINGUSERAPI DWORD EVERYTHINGAPI Everything_GetReplyID(void);
EVERYTHINGUSERAPI DWORD EVERYTHINGAPI Everything_GetSort(void); // Everything 1.4.1
EVERYTHINGUSERAPI DWORD EVERYTHINGAPI Everything_GetRequestFlags(void); // Everything 1.4.1
*/

void Py_Everything_Cleanup() 
{
	Everything_CleanUp();
}

BOOL Py_Everything_GetMatchPath()
{
	return Everything_GetMatchPath();
}

BOOL Py_Everything_GetMatchCase()
{
	return Everything_GetMatchCase();
}

BOOL Py_Everything_GetMatchWholeWord()
{
	return Everything_GetMatchWholeWord();
}

BOOL Py_Everything_GetRegex()
{
	return Everything_GetRegex();
}

DWORD Py_Everything_GetMax()
{
	return Everything_GetMax();
}

DWORD Py_Everything_GetOffset()
{
	return Everything_GetOffset();
}

std::wstring Py_Everything_GetSearch()
{
	auto rv = Everything_GetSearch();
	checkEverythingErrorCodeAndThrow(rv);
	return std::wstring(rv);
}

DWORD Py_Everything_GetLastError() 
{
	return Everything_GetLastError();
}

HWND Py_Everything_GetReplyWindow()
{
	return Everything_GetReplyWindow();
}

DWORD Py_Everything_GetReplyID()
{
	return Everything_GetReplyID();
}

DWORD Py_Everything_GetSort()
{
	return Everything_GetReplyID();
}

DWORD Py_Everything_GetRequestFlags()
{
	return Everything_GetRequestFlags();
}

BOOL Py_Everything_IsQueryReply(UINT message, WPARAM wParam, LPARAM lParam, DWORD nID)
{
	auto rv = Everything_IsQueryReply(message, wParam, lParam, nID);
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

void Py_Everything_SortResultsByPath()
{
	Everything_SortResultsByPath();
}

DWORD	Py_Everything_GetNumFileResults(void)
{
	DWORD num = Everything_GetNumFileResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

DWORD	Py_Everything_GetNumFolderResults(void)
{
	DWORD num = Everything_GetNumFolderResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

DWORD	Py_Everything_GetNumResults(void)
{
	DWORD num = Everything_GetNumResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

DWORD	Py_Everything_GetTotFileResults(void)
{
	DWORD num = Everything_GetTotFileResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

DWORD	Py_Everything_GetTotFolderResults(void)
{
	DWORD num = Everything_GetTotFolderResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

DWORD	Py_Everything_GetTotResults(void)
{
	DWORD num = Everything_GetTotResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

BOOL	Py_Everything_IsVolumeResult(DWORD index)
{
	BOOL isVolume = Everything_IsVolumeResult(index);
	checkEverythingErrorCodeAndThrow(isVolume);
	return isVolume;
}

BOOL	Py_Everything_IsFolderResult(DWORD index)
{
	BOOL isFolder = Everything_IsFolderResult(index);
	checkEverythingErrorCodeAndThrow(isFolder);
	return isFolder;
}

BOOL	Py_Everything_IsFileResult(DWORD index)
{
	BOOL isFile = Everything_IsFileResult(index);
	checkEverythingErrorCodeAndThrow(isFile);
	return isFile;
}

std::wstring Py_Everything_GetResultFileName(DWORD index)
{
	auto result = Everything_GetResultFileName(index);
	checkEverythingErrorCodeAndThrow(result);
	return std::wstring(result);
}

std::wstring Py_Everything_GetResultPath(DWORD index)
{
	auto result = Everything_GetResultPath(index);
	checkEverythingErrorCodeAndThrow(result);
	return std::wstring(result);
}

std::wstring Py_Everything_GetResultFullPathName(DWORD index, const DWORD maxCount = MAX_PATH) 
{
	std::vector<wchar_t> buf = std::vector<wchar_t>(maxCount);
	int ok = Everything_GetResultFullPathName(index, &buf[0], maxCount);
	checkEverythingErrorCodeAndThrow(ok);
	const std::wstring result = std::wstring(&buf[0]);
	return result;
}

DWORD	Py_Everything_GetResultListSort(void); // Everything 1.4.1
DWORD	Py_Everything_GetResultListRequestFlags(void); // Everything 1.4.1
LPCWSTR Py_Everything_GetResultExtension(DWORD dwIndex); // Everything 1.4.1
BOOL	Py_Everything_GetResultSize(DWORD dwIndex); // Everything 1.4.1
BOOL	Py_Everything_GetResultDateCreated(DWORD dwIndex); // Everything 1.4.1
BOOL	Py_Everything_GetResultDateModified(DWORD dwIndex); // Everything 1.4.1
BOOL	Py_Everything_GetResultDateAccessed(DWORD dwIndex); // Everything 1.4.1
DWORD	Py_Everything_GetResultAttributes(DWORD dwIndex); // Everything 1.4.1
LPCWSTR Py_Everything_GetResultFileListFileName(DWORD dwIndex); // Everything 1.4.1
DWORD	Py_Everything_GetResultRunCount(DWORD dwIndex); // Everything 1.4.1
BOOL	Py_Everything_GetResultDateRun(DWORD dwIndex);
BOOL	Py_Everything_GetResultDateRecentlyChanged(DWORD dwIndex);
LPCWSTR Py_Everything_GetResultHighlightedFileName(DWORD dwIndex); // Everything 1.4.1
LPCWSTR Py_Everything_GetResultHighlightedPath(DWORD dwIndex); // Everything 1.4.1
LPCWSTR Py_Everything_GetResultHighlightedFullPathAndFileName(DWORD dwIndex); // Everything 1.4.1

void Py_Everything_SetSearch(const std::wstring& searchTerm) 
{
	Everything_SetSearch(searchTerm.c_str());
}

void Py_Everything_SetMatchPath(const bool enable)
{
	Everything_SetMatchPath(enable);
}

void Py_Everything_SetMatchCase(const bool enable)
{
	Everything_SetMatchCase(enable);
}

void Py_Everything_SetMatchWholeWord(const bool enable)
{
	Everything_SetMatchWholeWord(enable);
}

void Py_Everything_SetRegex(const bool enable)
{
	Everything_SetRegex(enable);
}

void Py_Everything_SetReplyWindow(const HWND windowId)
{
	Everything_SetReplyWindow(windowId);
}

void Py_Everything_SetReplyID(const DWORD replyID)
{
	Everything_SetReplyID(replyID);
}

void Py_Everything_SetRequestFlags(const DWORD requestFlags)
{
	Everything_SetRequestFlags(requestFlags);
}

void Py_Everything_SetOffset(DWORD offset) 
{
	Everything_SetOffset(offset);
}

void Py_Everything_SetMax(DWORD maxResults) 
{
	Everything_SetMax(maxResults);
}

void Py_Everything_SetSort(DWORD sortType) 
{
	Everything_SetSort(sortType);
}

void Py_Everything_Query(bool wait = true) 
{
	int ok = Everything_Query(wait);
	checkEverythingErrorCodeAndThrow(ok);
}

PYBIND11_MODULE(Everything_Python, m) 
{
	m.doc() = "Everything Python Bindings.";

	//Functions

	//Setting search state
	m.def("SetSearch", &Py_Everything_SetSearch, py::arg("search_string"));
	m.def("SetMatchPath", &Py_Everything_SetMatchPath, py::arg("enable"));
	m.def("SetMatchCase", &Py_Everything_SetMatchCase, py::arg("enable"));
	m.def("SetMatchWholeWord", &Py_Everything_SetMatchWholeWord, py::arg("enable"));
	m.def("SetMatchCase", &Py_Everything_SetRegex, py::arg("enable"));
	m.def("SetMax", &Py_Everything_SetMax, py::arg("max"));
	m.def("SetOffset", &Py_Everything_SetOffset, py::arg("offset"));
	m.def("SetReplyWindow", &Py_Everything_SetReplyWindow, py::arg("reply_window_handle"));
	m.def("SetReplyID", &Py_Everything_SetReplyID, py::arg("reply_id"));
	m.def("SetSort", &Py_Everything_SetSort, py::arg("sort_mode"));
	m.def("SetRequestFlags", &Py_Everything_SetRequestFlags, py::arg("request_flags"));
	
	//reading search state
	m.def("GetMatchPath", &Py_Everything_GetMatchPath);
	m.def("GetMatchCase", &Py_Everything_GetMatchCase);
	m.def("GetMatchWholeWord", &Py_Everything_GetMatchWholeWord);
	m.def("GetRegex", &Py_Everything_GetRegex);
	m.def("GetMax", &Py_Everything_GetMax);
	m.def("GetOffset", &Py_Everything_GetOffset);
	m.def("GetSearch", &Py_Everything_GetSearch);
	m.def("GetLastError", &Py_Everything_GetLastError);
	m.def("GetReplyWindow", &Py_Everything_GetReplyWindow);
	m.def("GetReplyID", &Py_Everything_GetReplyID);
	m.def("GetSort", &Py_Everything_GetSort);
	m.def("GetRequestFlags", &Py_Everything_GetRequestFlags);

	m.def("IsQueryReply", &Py_Everything_IsQueryReply, py::arg("message"), py::arg("wparam"), py::arg("lparam"), py::arg("dwID"));
	m.def("SortResultsByPath", &Py_Everything_SortResultsByPath); // might actually be deprecated in favour of Everything_SetSort, need to check.
	
	m.def("GetNumFileResults", &Py_Everything_GetNumFileResults);
	m.def("GetNumFolderResults", &Py_Everything_GetNumFolderResults);
	m.def("GetNumResults", &Py_Everything_GetNumResults);
	m.def("GetTotFileResults", &Py_Everything_GetTotFileResults);
	m.def("GetTotFolderResults", &Py_Everything_GetTotFolderResults);
	m.def("GetTotResults", &Py_Everything_GetTotResults);
	m.def("IsVolumeResult", &Py_Everything_IsVolumeResult);
	m.def("IsFolderResult", &Py_Everything_IsFolderResult);
	m.def("IsFileResult", &Py_Everything_IsFileResult);
	m.def("GetResultFileName", &Py_Everything_GetResultFileName, py::arg("index"));
	m.def("GetResultPath", &Py_Everything_GetResultPath, py::arg("index"));
	m.def("GetResultFullPathName", &Py_Everything_GetResultFullPathName, py::arg("index"), py::arg_v("max_path_length", (DWORD)MAX_PATH));
	//m.def("GetResultListSort", &Py_Everything_GetResultListSort);
	//m.def("GetResultListRequestFlags", &Py_Everything_GetResultListRequestFlags);
	//m.def("GetResultExtension", &Py_Everything_GetResultExtension, py::arg("index"));
	//m.def("GetResultSize", &Py_Everything_GetResultSize, py::arg("index"));
	//m.def("GetResultDateCreated", &Py_Everything_GetResultDateCreated, py::arg("index"));
	//m.def("GetResultDateModified", &Py_Everything_GetResultDateModified, py::arg("index"));
	//m.def("GetResultDateAccessed", &Py_Everything_GetResultDateAccessed, py::arg("index"));
	//m.def("GetResultAttributes", &Py_Everything_GetResultAttributes, py::arg("index"));
	//m.def("GetResultFileListFileName", &Py_Everything_GetResultFileListFileName, py::arg("index"));
	//m.def("GetResultRunCount", &Py_Everything_GetResultRunCount, py::arg("index"));
	//m.def("GetResultDateRun", &Py_Everything_GetResultDateRun, py::arg("index"));
	//m.def("GetResultDateRecentlyChanged", &Py_Everything_GetResultDateRecentlyChanged, py::arg("index"));
	//m.def("GetResultHighlightedFileName", &Py_Everything_GetResultHighlightedFileName, py::arg("index"));
	//m.def("GetResultHighlightedPath", &Py_Everything_GetResultHighlightedPath, py::arg("index"));
	//m.def("GetResultHighlightedFullPathAndFileName", &Py_Everything_GetResultHighlightedFullPathAndFileName, py::arg("index"));

	//The big one, run the query with the current search state	
	m.def("Query", &Py_Everything_Query, py::arg("wait"));

	//Members

	//error codes
	m.attr("EVERYTHING_OK") = py::int_(EVERYTHING_OK);												// 0 no error detected
	m.attr("EVERYTHING_ERROR_MEMORY") = py::int_(EVERYTHING_ERROR_MEMORY);							// 1 out of memory.
	m.attr("EVERYTHING_ERROR_IPC") = py::int_(EVERYTHING_ERROR_IPC);								// 2 Everything search client is not running
	m.attr("EVERYTHING_ERROR_REGISTERCLASSEX") = py::int_(EVERYTHING_ERROR_REGISTERCLASSEX);		// 3 unable to register window class.
	m.attr("EVERYTHING_ERROR_CREATEWINDOW") = py::int_(EVERYTHING_ERROR_CREATEWINDOW);				// 4 unable to create listening window
	m.attr("EVERYTHING_ERROR_CREATETHREAD") = py::int_(EVERYTHING_ERROR_CREATETHREAD);				// 5 unable to create listening thread
	m.attr("EVERYTHING_ERROR_INVALIDINDEX") = py::int_(EVERYTHING_ERROR_INVALIDINDEX);				// 6 invalid index
	m.attr("EVERYTHING_ERROR_INVALIDCALL") = py::int_(EVERYTHING_ERROR_INVALIDCALL);				// 7 invalid call
	m.attr("EVERYTHING_ERROR_INVALIDREQUEST") = py::int_(EVERYTHING_ERROR_INVALIDREQUEST);			// 8 invalid request data, request data first.
	m.attr("EVERYTHING_ERROR_INVALIDPARAMETER") = py::int_(EVERYTHING_ERROR_INVALIDPARAMETER);		// 9 bad parameter.

	//Search request flags
	m.attr("REQUEST_FILE_NAME") = py::int_(EVERYTHING_REQUEST_FILE_NAME);
	m.attr("REQUEST_PATH") = py::int_(EVERYTHING_REQUEST_PATH);
	m.attr("REQUEST_FULL_PATH_AND_FILE_NAME") = py::int_(EVERYTHING_REQUEST_FULL_PATH_AND_FILE_NAME);
	m.attr("REQUEST_EXTENSION") = py::int_(EVERYTHING_REQUEST_EXTENSION);
	m.attr("REQUEST_SIZE") = py::int_(EVERYTHING_REQUEST_SIZE);
	m.attr("REQUEST_DATE_CREATED") = py::int_(EVERYTHING_REQUEST_DATE_CREATED);
	m.attr("REQUEST_DATE_MODIFIED") = py::int_(EVERYTHING_REQUEST_DATE_MODIFIED);
	m.attr("REQUEST_DATE_ACCESSED") = py::int_(EVERYTHING_REQUEST_DATE_ACCESSED);
	m.attr("REQUEST_ATTRIBUTES") = py::int_(EVERYTHING_REQUEST_ATTRIBUTES);
	m.attr("REQUEST_FILE_LIST_FILE_NAME") = py::int_(EVERYTHING_REQUEST_FILE_LIST_FILE_NAME);
	m.attr("REQUEST_RUN_COUNT") = py::int_(EVERYTHING_REQUEST_RUN_COUNT);
	m.attr("REQUEST_DATE_RUN") = py::int_(EVERYTHING_REQUEST_DATE_RUN);
	m.attr("REQUEST_DATE_RECENTLY_CHANGED") = py::int_(EVERYTHING_REQUEST_DATE_RECENTLY_CHANGED);
	m.attr("REQUEST_HIGHLIGHTED_FILE_NAME") = py::int_(EVERYTHING_REQUEST_HIGHLIGHTED_FILE_NAME);
	m.attr("REQUEST_HIGHLIGHTED_PATH") = py::int_(EVERYTHING_REQUEST_HIGHLIGHTED_PATH);
	m.attr("REQUEST_HIGHLIGHTED_FULL_PATH_AND_FILE_NAME") = py::int_(EVERYTHING_REQUEST_HIGHLIGHTED_FULL_PATH_AND_FILE_NAME);

	//Result sort flags
	m.attr("SORT_NAME_ASCENDING")= py::int_(EVERYTHING_SORT_NAME_ASCENDING);
	m.attr("SORT_NAME_DESCENDING") = py::int_(EVERYTHING_SORT_NAME_DESCENDING);
	m.attr("SORT_PATH_ASCENDING") = py::int_(EVERYTHING_SORT_PATH_ASCENDING);
	m.attr("SORT_PATH_DESCENDING") = py::int_(EVERYTHING_SORT_PATH_DESCENDING);
	m.attr("SORT_SIZE_ASCENDING") = py::int_(EVERYTHING_SORT_SIZE_ASCENDING);
	m.attr("SORT_SIZE_DESCENDING") = py::int_(EVERYTHING_SORT_SIZE_DESCENDING);
	m.attr("SORT_EXTENSION_ASCENDING") = py::int_(EVERYTHING_SORT_EXTENSION_ASCENDING);
	m.attr("SORT_EXTENSION_DESCENDING") = py::int_(EVERYTHING_SORT_EXTENSION_DESCENDING);
	m.attr("SORT_TYPE_NAME_ASCENDING") = py::int_(EVERYTHING_SORT_TYPE_NAME_ASCENDING);
	m.attr("SORT_TYPE_NAME_DESCENDING") = py::int_(EVERYTHING_SORT_TYPE_NAME_DESCENDING);
	m.attr("SORT_DATE_CREATED_ASCENDING") = py::int_(EVERYTHING_SORT_DATE_CREATED_ASCENDING);
	m.attr("SORT_DATE_CREATED_DESCENDING") = py::int_(EVERYTHING_SORT_DATE_CREATED_DESCENDING);
	m.attr("SORT_DATE_MODIFIED_ASCENDING") = py::int_(EVERYTHING_SORT_DATE_MODIFIED_ASCENDING);
	m.attr("SORT_DATE_MODIFIED_DESCENDING") = py::int_(EVERYTHING_SORT_DATE_MODIFIED_DESCENDING);
	m.attr("SORT_ATTRIBUTES_ASCENDING") = py::int_(EVERYTHING_SORT_ATTRIBUTES_ASCENDING);
	m.attr("SORT_ATTRIBUTES_DESCENDING") = py::int_(EVERYTHING_SORT_ATTRIBUTES_DESCENDING);
	m.attr("SORT_FILE_LIST_FILENAME_ASCENDING") = py::int_(EVERYTHING_SORT_FILE_LIST_FILENAME_ASCENDING);
	m.attr("SORT_FILE_LIST_FILENAME_DESCENDING") = py::int_(EVERYTHING_SORT_FILE_LIST_FILENAME_DESCENDING);
	m.attr("SORT_RUN_COUNT_ASCENDING") = py::int_(EVERYTHING_SORT_RUN_COUNT_ASCENDING);
	m.attr("SORT_RUN_COUNT_DESCENDING") = py::int_(EVERYTHING_SORT_RUN_COUNT_DESCENDING);
	m.attr("SORT_DATE_RECENTLY_CHANGED_ASCENDING") = py::int_(EVERYTHING_SORT_DATE_RECENTLY_CHANGED_ASCENDING);
	m.attr("SORT_DATE_RECENTLY_CHANGED_DESCENDING") = py::int_(EVERYTHING_SORT_DATE_RECENTLY_CHANGED_DESCENDING);
	m.attr("SORT_DATE_ACCESSED_ASCENDING") = py::int_(EVERYTHING_SORT_DATE_ACCESSED_ASCENDING);
	m.attr("SORT_DATE_ACCESSED_DESCENDING") = py::int_(EVERYTHING_SORT_DATE_ACCESSED_DESCENDING);
	m.attr("SORT_DATE_RUN_ASCENDING") = py::int_(EVERYTHING_SORT_DATE_RUN_ASCENDING);
	m.attr("SORT_DATE_RUN_DESCENDING") = py::int_(EVERYTHING_SORT_DATE_RUN_DESCENDING);
}