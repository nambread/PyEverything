// Everything-Python.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <locale>
#include <codecvt>

#include "Everything.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;

std::wstring Py_Test_Everything_Search(const std::wstring& searchString) {
	Everything_SetSearch(searchString.c_str());
	int ok = Everything_Query(true);
	if (ok && Everything_GetNumResults() > 0) {
		Everything_GetResultPath(0);
		TCHAR buf[MAX_PATH];
		Everything_GetResultFullPathName(0, buf, sizeof(buf) / sizeof(TCHAR));
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


PYBIND11_MODULE(Everything_Python, m) {
	m.doc() = "Everything Python Bindings.";
	m.def("TestSearch", &Py_Test_Everything_Search, py::arg("searchTerm"));
}