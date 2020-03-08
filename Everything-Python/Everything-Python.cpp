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
struct EverythingException : std::exception {

	const UINT32 m_errorCode;
	EverythingException(const UINT32& errorCode) : m_errorCode(errorCode) {}

	std::string errCodeString = std::to_string(this->m_errorCode);
	const std::string m_errorMessage = "Everything API returned error code: " + errCodeString;
	const char* what() const noexcept { return m_errorMessage.c_str(); }
};

/*
Converts the error codes into exceptions that get thrown.
@param status The status returned from various api calls. Most will return a 0 if the function fails.
Gets a bit confusing in cases where the return value is also used as a count, where 0 is a valid value.
*/
static inline void checkEverythingErrorCodeAndThrow(const int& status) {
	if (status == 0) {
		UINT32 errorCode = Everything_GetLastError();
		if (errorCode) {
			//sometimes 0 is a valid status e.g when returning number of result matches.
			throw EverythingException(errorCode);
		}
	}
}

std::wstring Py_Test_Everything_Search(const std::wstring& searchString) {
	Everything_SetSearch(searchString.c_str());
	int ok = Everything_Query(true);
	checkEverythingErrorCodeAndThrow(ok);
	if (Everything_GetNumResults() > 0) {
		TCHAR buf[MAX_PATH];
		int ok = Everything_GetResultFullPathName(0, buf, sizeof(buf) / sizeof(TCHAR));
		checkEverythingErrorCodeAndThrow(ok);
		const std::wstring result = std::wstring(buf);
		return result;
	}
	else {
		return L"";
	}
}

void Py_Everything_Cleanup() {
	Everything_CleanUp();
}

UINT32 Py_Everything_GetLastError() {
	return Everything_GetLastError();
}

UINT32 Py_Everything_GetTotResults() {
	UINT32 num = Everything_GetTotResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

UINT32 Py_Everything_GetNumResults() {
	UINT32 num = Everything_GetNumResults();
	checkEverythingErrorCodeAndThrow(num);
	return num;
}

std::wstring Py_Everything_GetResultFullPathName(UINT32 index, const UINT32 maxCount = MAX_PATH) {
	std::vector<wchar_t> buf = std::vector<wchar_t>(maxCount);
	int ok = Everything_GetResultFullPathName(index, &buf[0], maxCount);
	checkEverythingErrorCodeAndThrow(ok);
	const std::wstring result = std::wstring(&buf[0]);
	return result;
}

void Py_Everything_SetSearch(const std::wstring& searchTerm) {
	Everything_SetSearch(searchTerm.c_str());
}

void Py_Everything_SetOffset(UINT32 offset) {
	Everything_SetOffset(offset);
}

void Py_Everything_SetMax(UINT32 maxResults) {
	Everything_SetMax(maxResults);
}

void Py_Everything_SetSort(UINT32 sortType) {
	Everything_SetSort(sortType);
}

void Py_Everything_Query(bool wait = true) {
	int ok = Everything_Query(wait);
	checkEverythingErrorCodeAndThrow(ok);
}

PYBIND11_MODULE(Everything_Python, m) {
	m.doc() = "Everything Python Bindings.";

	//Functions
	m.def("GetLastError", &Py_Everything_GetLastError);
	m.def("GetNumResults", &Py_Everything_GetNumResults);
	m.def("GetResultFullPathName", &Py_Everything_GetResultFullPathName, py::arg("index"), py::arg("max_count"));
	m.def("SetMax", &Py_Everything_SetMax);
	m.def("SetOffset", &Py_Everything_SetOffset);
	m.def("SetSearch", &Py_Everything_SetSearch, py::arg("search_term"));
	m.def("Query", &Py_Everything_Query, py::arg("wait"));

	//Members

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