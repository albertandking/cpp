add_library(option option.cpp option.h py_option.cpp)

find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(option PRIVATE pybind11::lto pybind11::embed pybind11::module pybind11::headers)
