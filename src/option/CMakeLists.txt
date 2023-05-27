add_library(option option.cpp option.h py_option.cpp)

find_library(QUANTLIB_LIBRARY QuantLib PATHS "C:/Users/albert/Desktop/workhome/QuantLib/lib")
find_path(QUANTLIB_INCLUDE_DIR ql/quantlib.hpp PATHS "C:/Users/albert/Desktop/workhome/QuantLib")

find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(option PRIVATE ${QUANTLIB_LIBRARY} pybind11::lto pybind11::embed pybind11::module pybind11::headers)
target_include_directories(option PUBLIC ${QUANTLIB_INCLUDE_DIR})
