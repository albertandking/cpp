cmake_minimum_required(VERSION 3.25)

project(cpp)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/packages)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/packages)
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/packages)
endforeach()

# 设置 vcpkg 的头文件查找路径
include_directories("C:/Users/albert/.clion-vcpkg/vcpkg/installed/x64-windows/include")
include_directories("C:/Users/albert/.conda/envs/pb/Lib/site-packages/pybind11")

# 设置 Python 查找路径
set(ENV{CONDA_PREFIX} "C:/Users/albert/.conda/envs/pb")
find_package(Python REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python_FOUND: ${Python_FOUND}")
message(STATUS "Python_EXECUTABLE: ${Python_EXECUTABLE}")
message(STATUS "Python_INCLUDE_DIRS: ${Python_INCLUDE_DIRS}")
message(STATUS "Python_LIBRARIES: ${Python_LIBRARIES}")

# 指定 Quantlib 的头文件及库文件的路径
include_directories("C:/Users/albert/Desktop/workhome/QuantLib")
link_directories("C:/Users/albert/Desktop/workhome/QuantLib/lib")

# 设置 Pybind11 的查找路径
set(pybind11_DIR  C:/Users/albert/.conda/envs/pb/lib/site-packages/pybind11/share/cmake/pybind11)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(example src/example.cpp)
pybind11_add_module(add src/add.cpp)
pybind11_add_module(options src/option/py_option.cpp)

# 此处 options 项目需要依靠 option 这个库
target_link_libraries(options PUBLIC QuantLib pybind11::lto pybind11::embed pybind11::module pybind11::headers option)
target_link_libraries(example PUBLIC QuantLib pybind11::lto pybind11::embed pybind11::module pybind11::headers)

# 新增子项目
add_subdirectory(src/option)

# 添加并生成一个可执行程序
add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/src/option)
# 指定要链接的动态库
target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib pybind11::lto pybind11::embed pybind11::module pybind11::headers option)
