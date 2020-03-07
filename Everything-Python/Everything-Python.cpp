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

inline std::wstring convert(const std::string& as)
{
	// deal with trivial case of empty string
	if (as.empty())    return std::wstring();

	// determine required length of new string
	size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

	// construct new string of required length
	std::wstring ret(reqLength, L'\0');

	// convert old string to new string
	::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

	// return new string ( compiler should optimize this away )
	return ret;
}

std::wstring Py_Everything_Search(std::string searchString) {
	size_t searchSize = searchString.size() * 2 + 2;
	wchar_t* searchTerm = new wchar_t[searchSize];
	swprintf(searchTerm, searchSize, L"%S", searchString.c_str());
	Everything_SetSearch(searchTerm);
	bool ok = Everything_Query(true);
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

PYBIND11_MODULE(Everything_Python, m) {
	m.doc() = "Everything Python Bindings.";
	m.def("TestSearch", &Py_Everything_Search, py::arg("searchTerm"));
}