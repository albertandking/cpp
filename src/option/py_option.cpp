//
// Created by albert on 2023/5/26.
//
#include <pybind11/pybind11.h>
#include "option.h"


PYBIND11_MODULE(options, m) {
m.doc() = "pybind11 example plugin"; // optional module docstring

m.def("cal_option", &cal_option, "A function that adds two numbers");
}
