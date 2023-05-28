## 注意事项

此处主要描述我们在使用该项目时候，需要注意的内容。

### Python

注意 Python 版本的问题，我们建议使用 Python 3.11.x 来进行编程，因为其由较大的性能提升


### 如何编译安装 TA-Lib

#### 下载源码

TA-LIB 官网：[TA-LIB 官网地址](http://www.ta-lib.org/hdr_dw.html)

#### 编译

由于 TA-LIB 只有 32 位的编译后的版本，而没有 64 位的编译好的版本，所以我们需要自行编译，如何直接使用会导致不兼容的问题

下载 ta-lib-0.4.0-msvc.zip 安装包，解压到任意目录下
比如：`C:\Users\albert\Desktop\workhome\ta-lib` 目录
然后打开 `x64 Native Tools Command Prompt for VS 2022` 文件，这里的 64 就是指编译 64 位的程序，切换到该目录下，执行以下命令：

```cmd
nmake
```

就会在 `C:\Users\albert\Desktop\workhome\ta-lib\lib` 目录下生成 `ta_libc_cdr.lib` 文件等 4 个文件
将这个 5 个文件 include 进库就可以了

在 CMakeLists.txt 中添加以下内容：

```cmake
# TA-Lib
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ta-lib/lib/ta_libc_cdr.lib)
```

其中 `ta_libc_cdr.lib` 必须和编译出来的 64 位程序名字是一样的，另外 3 个不用链接，但需要放在同一个目录下
