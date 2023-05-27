#include <iostream>
#include "pybind11/pybind11.h"


int add(int a, int b){
    return a + b;
}

PYBIND11_MODULE(add, m) {
m.doc() = "pybind11 example plugin"; // optional module docstring

m.def("add", &add, "A function that adds two numbers");
}
