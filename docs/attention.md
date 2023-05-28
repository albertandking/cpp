## 注意事项

此处主要描述我们在使用该项目时候，需要注意的内容。

### Python

注意 Python 版本的问题，我们建议使用 Python 3.11.x 来进行编程，因为其有较大的性能提升，推荐大家使用，至于 64 位还是 32 位的问题，
在 2023 年，默认 64 位是最佳选择（除非是做嵌入式等特殊领域的用户）

### 如何编译安装 TA-Lib

Technical Analysis Library 是一个由 C 语言编写的技术分析库，我们可以使用它来计算各种技术指标，比如 MACD、KDJ、RSI 等等，
但是其由于开发的时间比较早，所以其只有 32 位的编译版本，而没有 64 位的编译版本，所以我们需要自行编译。下面给大家介绍如何在 windows11
64 位操作系统上进行编译。

#### 下载源码

TA-LIB 官网：[TA-LIB 官网地址](http://www.ta-lib.org/hdr_dw.html)，我们只需要下载 `ta-lib-0.4.0-msvc.zip` 即可！

#### 编译

下载 `ta-lib-0.4.0-msvc.zip` 安装包，解压到任意目录下，比如：`C:\Users\albert\Desktop\workhome\ta-lib` 目录
然后打开 `x64 Native Tools Command Prompt for VS 2022` 文件，这里的 64 就是指编译 64 位的程序，切换到该目录下，执行以下命令：

```cmd
nmake
```

就会在 `C:\Users\albert\Desktop\workhome\ta-lib\lib` 目录下生成 `ta_libc_cdr.lib` 文件等 4 个文件，然后将这些文件 include 进库就可以了

在 CMakeLists.txt 中，除了需要 include 头文件外，还需要加以下内容：

```cmake
# TA-Lib
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ta-lib/lib/ta_libc_cdr.lib)
```

其中 `ta_libc_cdr.lib` 必须和编译出来的 64 位程序名字是一样的，另外 3 个不用链接，但需要放在同一个目录下

### 关于 Python 安装 talib

由于 Python 安装，需要预先编译 `ta-lib-0.4.0-msvc.zip` 到 c 盘目录下，因为 talib 这个 Python 库在安装时，会自动寻找
c 盘下面的 `ta-lib`，例如：`c:/ta-lib`，所以我们必须要在这个目录编译出来后，才可以在系统进行安装。

