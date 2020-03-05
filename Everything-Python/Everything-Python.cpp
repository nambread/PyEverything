// Everything-Python.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Everything.h"
#include "pybind11/pybind11.h"

int main()
{
	const wchar_t* cheese = L"Cheese";
	Everything_SetSearch(cheese);
}

PYBIND11_MODULE(ex, m) {
	m.doc() = "Testing";
}