// Everything-Python.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <locale>
#include <codecvt>
#include <chrono>

#include "Everything.h"
#include "pybind11/pybind11.h"
#include "pybind11/chrono.h"

#ifdef _MSC_VER
	#pragma comment(lib, "user32")
#endif

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

	const std::string errCodeString = std::to_string(this->m_errorCode);
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

//the system_clock::time_point conversion uses an unsafe std::localtime function. Disable the warning with _CRT_SECURE_NO_WARNINGS
//Will implement our own FILETIME > datetime conversion function if this causes problems.

static inline auto FILETIMEtoTime_Point(const FILETIME& ft) 
{
	time_t t;
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;
	t = ull.QuadPart / 10000000ULL - 11644473600ULL; //https://cyberspock.com/2015/10/02/some-time_point-to-from-filetime-conversions/ something to do with clock tick resolution and epoch adjustment.
	return std::chrono::system_clock::from_time_t(t);
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
	return Everything_GetSort();
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

DWORD	Py_Everything_GetResultListSort(void)
{
	return Everything_GetResultListSort();
}

DWORD	Py_Everything_GetResultListRequestFlags(void)
{
	return Everything_GetResultListRequestFlags();
}

std::wstring Py_Everything_GetResultExtension(DWORD index)
{
	auto rv = Everything_GetResultExtension(index);
	checkEverythingErrorCodeAndThrow(rv);
	return std::wstring(rv);
}

LARGE_INTEGER Py_Everything_GetResultSize(DWORD index)
{
	LARGE_INTEGER size;
	auto ok = Everything_GetResultSize(index, &size);
	checkEverythingErrorCodeAndThrow(ok);
	return size;
}

auto Py_Everything_GetResultDateCreated(DWORD index)
{
	FILETIME time;
	auto ok = Everything_GetResultDateCreated(index, &time);
	checkEverythingErrorCodeAndThrow(ok);
	return FILETIMEtoTime_Point(time);
}

auto Py_Everything_GetResultDateModified(DWORD index)
{
	FILETIME time;
	auto ok = Everything_GetResultDateModified(index, &time);
	checkEverythingErrorCodeAndThrow(ok);
	return FILETIMEtoTime_Point(time);
}

auto Py_Everything_GetResultDateAccessed(DWORD index)
{
	FILETIME time;
	auto ok = Everything_GetResultDateAccessed(index, &time);
	checkEverythingErrorCodeAndThrow(ok);
	return FILETIMEtoTime_Point(time);
}

DWORD	Py_Everything_GetResultAttributes(DWORD index)
{
	auto rv = Everything_GetResultAttributes(index);
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

std::wstring Py_Everything_GetResultFileListFileName(DWORD index)
{
	auto rv = Everything_GetResultFileListFileName(index);
	checkEverythingErrorCodeAndThrow(rv);
	return std::wstring(rv);
}

DWORD	Py_Everything_GetResultRunCount(DWORD index)
{
	auto rv = Everything_GetResultRunCount(index);
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

auto Py_Everything_GetResultDateRun(DWORD index)
{
	FILETIME time;
	auto ok = Everything_GetResultDateRun(index, &time);
	checkEverythingErrorCodeAndThrow(ok);
	return FILETIMEtoTime_Point(time);
}

auto Py_Everything_GetResultDateRecentlyChanged(DWORD index)
{
	FILETIME time;
	auto ok = Everything_GetResultDateRecentlyChanged(index, &time);
	checkEverythingErrorCodeAndThrow(ok);
	return FILETIMEtoTime_Point(time);
}

std::wstring Py_Everything_GetResultHighlightedFileName(DWORD index)
{
	auto rv = Everything_GetResultHighlightedFileName(index);
	checkEverythingErrorCodeAndThrow(rv);
	return std::wstring(rv);
}

std::wstring Py_Everything_GetResultHighlightedPath(DWORD index)
{
	auto rv = Everything_GetResultHighlightedPath(index);
	checkEverythingErrorCodeAndThrow(rv);
	return std::wstring(rv);
}

std::wstring Py_Everything_GetResultHighlightedFullPathAndFileName(DWORD index)
{
	auto rv = Everything_GetResultHighlightedFullPathAndFileName(index);
	checkEverythingErrorCodeAndThrow(rv);
	return std::wstring(rv);
}

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

DWORD Py_Everything_GetMajorVersion(void)
{
	auto rv = Everything_GetMajorVersion();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

DWORD Py_Everything_GetMinorVersion(void)
{
	auto rv = Everything_GetMinorVersion();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

DWORD Py_Everything_GetRevision(void)
{
	auto rv = Everything_GetRevision();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

DWORD Py_Everything_GetBuildNumber(void)
{
	auto rv = Everything_GetBuildNumber();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_Exit(void)
{
	auto rv = Everything_Exit();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_IsDBLoaded(void)
{
	auto rv = Everything_IsDBLoaded();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_IsAdmin(void)
{
	auto rv = Everything_IsAdmin();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_IsAppData(void)
{
	auto rv = Everything_IsAppData();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_RebuildDB(void)
{
	auto rv = Everything_RebuildDB();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_UpdateAllFolderIndexes(void)
{
	auto rv = Everything_RebuildDB();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_SaveDB(void)
{
	auto rv = Everything_SaveDB();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_SaveRunHistory(void)
{
	auto rv = Everything_SaveRunHistory();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL Py_Everything_DeleteRunHistory(void)
{
	auto rv = Everything_DeleteRunHistory();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

DWORD Py_Everything_GetTargetMachine(void)
{
	auto rv = Everything_GetTargetMachine();
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

DWORD Py_Everything_GetRunCountFromFileName(const std::wstring fileName)
{
	auto rv = Everything_GetRunCountFromFileName(fileName.c_str());
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

BOOL  Py_Everything_SetRunCountFromFileName(const std::wstring fileName, DWORD runCount)
{
	auto rv = Everything_SetRunCountFromFileName(fileName.c_str(), runCount);
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}

DWORD Py_Everything_IncRunCountFromFileName(const std::wstring fileName)
{
	auto rv = Everything_IncRunCountFromFileName(fileName.c_str());
	checkEverythingErrorCodeAndThrow(rv);
	return rv;
}


PYBIND11_MODULE(_Everything_Python, m) 
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
	
	//reading query results
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
	m.def("GetResultListSort", &Py_Everything_GetResultListSort);
	m.def("GetResultListRequestFlags", &Py_Everything_GetResultListRequestFlags);
	m.def("GetResultExtension", &Py_Everything_GetResultExtension, py::arg("index"));
	m.def("GetResultSize", &Py_Everything_GetResultSize, py::arg("index"));
	m.def("GetResultDateCreated", &Py_Everything_GetResultDateCreated, py::arg("index"));
	m.def("GetResultDateModified", &Py_Everything_GetResultDateModified, py::arg("index"));
	m.def("GetResultDateAccessed", &Py_Everything_GetResultDateAccessed, py::arg("index"));
	m.def("GetResultAttributes", &Py_Everything_GetResultAttributes, py::arg("index"));
	m.def("GetResultFileListFileName", &Py_Everything_GetResultFileListFileName, py::arg("index"));
	m.def("GetResultRunCount", &Py_Everything_GetResultRunCount, py::arg("index"));
	m.def("GetResultDateRun", &Py_Everything_GetResultDateRun, py::arg("index"));
	m.def("GetResultDateRecentlyChanged", &Py_Everything_GetResultDateRecentlyChanged, py::arg("index"));
	m.def("GetResultHighlightedFileName", &Py_Everything_GetResultHighlightedFileName, py::arg("index"));
	m.def("GetResultHighlightedPath", &Py_Everything_GetResultHighlightedPath, py::arg("index"));
	m.def("GetResultHighlightedFullPathAndFileName", &Py_Everything_GetResultHighlightedFullPathAndFileName, py::arg("index"));

	//The big one, run the query with the current search state	
	m.def("Query", &Py_Everything_Query, py::arg_v("wait", true));

	m.def("Reset", &Everything_Reset);
	m.def("CleanUp", &Everything_CleanUp);

	m.def("GetMajorVersion", &Py_Everything_GetMajorVersion);
	m.def("GetMinorVersion", &Py_Everything_GetMinorVersion);
	m.def("GetRevision", &Py_Everything_GetRevision);
	m.def("GetBuildNumber", &Py_Everything_GetBuildNumber);
	m.def("Exit", &Py_Everything_Exit);
	m.def("IsDBLoaded", &Py_Everything_IsDBLoaded);
	m.def("IsAdmin", &Py_Everything_IsAdmin);
	m.def("IsAppData", &Py_Everything_IsAppData);
	m.def("RebuildDB", &Py_Everything_RebuildDB);
	m.def("UpdateAllFolderIndexes", &Py_Everything_UpdateAllFolderIndexes);
	m.def("SaveDB", &Py_Everything_SaveDB);
	m.def("SaveRunHistory", &Py_Everything_SaveRunHistory);
	m.def("DeleteRunHistory", &Py_Everything_DeleteRunHistory);
	m.def("GetTargetMachine", &Py_Everything_GetTargetMachine);

	//whatever run counts are, manipulate them here.
	m.def("GetRunCountFromFileName", &Py_Everything_GetRunCountFromFileName, py::arg("filepath"));
	m.def("SetRunCountFromFileName", &Py_Everything_SetRunCountFromFileName, py::arg("filepath"), py::arg("runCount"));
	m.def("IncRunCountFromFileName", &Py_Everything_IncRunCountFromFileName, py::arg("filepath"));

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

	//Target Machine flags

	m.attr("TARGET_MACHINE_X86") = py::int_(EVERYTHING_TARGET_MACHINE_X86);
	m.attr("TARGET_MACHINE_X64") = py::int_(EVERYTHING_TARGET_MACHINE_X64);
	m.attr("TARGET_MACHINE_ARM") = py::int_(EVERYTHING_TARGET_MACHINE_ARM);

}