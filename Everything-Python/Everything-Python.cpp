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

void Py_Everything_Query(bool wait = true) {
	int ok = Everything_Query(wait);
	checkEverythingErrorCodeAndThrow(ok);
}

PYBIND11_MODULE(Everything_Python, m) {
	m.doc() = "Everything Python Bindings.";
	m.def("GetLastError", &Py_Everything_GetLastError);
	m.def("GetNumResults", &Py_Everything_GetNumResults);
	m.def("GetResultFullPathName", &Py_Everything_GetResultFullPathName, py::arg("index"), py::arg("max_count"));
	m.def("SetSearch", &Py_Everything_SetSearch, py::arg("search_term"));
	m.def("Query", &Py_Everything_Query, py::arg("wait"));
}