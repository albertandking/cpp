# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.25)

# 设置项目名称
project(cpp VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准，此处主要使用 CPP11 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ENV{PATH} "C:/Users/albert/.conda/envs/pb;$ENV{PATH}")
message(STATUS "PATH: $ENV{PATH}")

# 设置输出目录
foreach (CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)  # 将当前正在处理的配置类型（${CONFIG_TYPE}）转换为大写字符串（${CONFIG_TYPE_UPPER}）
    # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/packages)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/lib)  # 设置当前配置类型的库文件（共享库和静态库）输出目录。
    # set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_SOURCE_DIR}/packages)
endforeach ()

# 设置 jsoncpp 的头文件及库文件的路径
find_package(jsoncpp CONFIG REQUIRED)

# 设置 TA-Lib 的头文件及库文件的路径
set(TA-Lib_INCLUDE_DIRS "C:/Users/albert/CLionProjects/cpp/include/ta-lib")
set(TA-Lib_LIBRARIES "C:/Users/albert/CLionProjects/cpp/lib/ta-lib")  # 要具体到 ta_libc_cdr.lib 库
message(STATUS "TA-Lib_INCLUDE_DIRS: ${TA-Lib_INCLUDE_DIRS}")
message(STATUS "TA-Lib_LIBRARIES: ${TA-Lib_LIBRARIES}")

# 设置 vcpkg 的头文件查找路径，如在 CLion 的 CMake 的环境变量中配置了，则不需要；而在 Powershell 运行则需要新增该环境变量，因为有时候会读取不到这句（当 CMake 缓存生成时）
#set(CMAKE_TOOLCHAIN_FILE "C:/Users/albert/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")

# 设置 Boost 的头文件及库文件的路径
find_package(Boost REQUIRED)
message(STATUS "Boost_FOUND: ${Boost_FOUND}")
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

# 设置 Python 查找路径
set(ENV{CONDA_PREFIX} "C:/Users/albert/.conda/envs/pb")
find_package(Python REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python_FOUND: ${Python_FOUND}")
message(STATUS "Python_EXECUTABLE: ${Python_EXECUTABLE}")
message(STATUS "Python_INCLUDE_DIRS: ${Python_INCLUDE_DIRS}")
message(STATUS "Python_LIBRARIES: ${Python_LIBRARIES}")
#link_directories(${Python_LIBRARIES})
#link_libraries(${Python_EXECUTABLE}")

# 指定 Quantlib 的头文件及库文件的路径
include_directories(${Boost_INCLUDE_DIRS})  # 由于 QuantLib 需要依赖 Boost，所以此处直接包含 boost 的目录
find_library(QUANTLIB_LIBRARY QuantLib PATHS "C:/Users/albert/Desktop/workhome/QuantLib/lib")
find_path(QUANTLIB_INCLUDE_DIR ql/quantlib.hpp PATHS "C:/Users/albert/Desktop/workhome/QuantLib")

# 设置 Pybind11 的查找路径，由于我们的 pybind11 在 conda 创建的虚拟环境中，所以此处需要指定到 camke 所在路径
set(pybind11_DIR C:/Users/albert/.conda/envs/pb/lib/site-packages/pybind11/share/cmake/pybind11)
find_package(pybind11 CONFIG REQUIRED)
message(STATUS "Found pybind11: ${pybind11_VERSION}")

pybind11_add_module(example src/example.cpp)
pybind11_add_module(add src/add.cpp)
pybind11_add_module(options src/option/py_option.cpp)

# 此处 options 项目需要依靠 option 这个库
target_link_libraries(options PUBLIC ${QUANTLIB_LIBRARY} pybind11::lto pybind11::embed pybind11::module pybind11::headers option)
target_link_libraries(example PUBLIC ${QUANTLIB_LIBRARY} Boost::headers pybind11::lto pybind11::embed pybind11::module pybind11::headers)
target_include_directories(example PUBLIC ${QUANTLIB_INCLUDE_DIR})

# 新增子项目
add_subdirectory(src/option)

# 添加并生成一个可执行程序
add_executable(${PROJECT_NAME} src/main.cpp src/version.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${TA-Lib_INCLUDE_DIRS} ${QUANTLIB_INCLUDE_DIR} ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/src/option)
# 指定要链接的动态库
target_link_libraries(${PROJECT_NAME} PUBLIC jsoncpp_lib ${TA-Lib_LIBRARIES}/ta_libc_cdr.lib ${QUANTLIB_LIBRARY} Boost::headers pybind11::lto pybind11::embed pybind11::module pybind11::headers option)
