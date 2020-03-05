// Everything-Python.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <locale>
#include <codecvt>

#include "Everything.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::string Py_Everything_Search(std::string searchString) {
	const wchar_t* searchTerm = std::wstring(searchString.begin(), searchString.end()).c_str();
	Everything_SetSearch(searchTerm);
	bool ok = Everything_Query(true);
	if (ok && Everything_GetNumResults() > 0) {
		Everything_GetResultPath(0);
		TCHAR buf[MAX_PATH];
		Everything_GetResultFullPathName(0, buf, sizeof(buf) / sizeof(TCHAR));
		const std::wstring result = std::wstring(buf);
		return ws2s(result);
	}
	else {
		return std::string("");
	}
}

PYBIND11_MODULE(EverythingPython, m) {
	m.doc() = "Everything Python Bindings.";
	m.def("TestSearch", &Py_Everything_Search, py::arg("searchTerm"));
}