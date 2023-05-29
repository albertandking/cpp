## 项目介绍

P.S. 本项目还处于玩具阶段，仅供参考！

本项目旨在通过 C++ 重写计算密集型代码来实现对 Python 程序的加速，本项目主要集成了 Boost 和 QuantLib 这两个库

## 项目

项目在 Windows-11 64 位操作系统中构建，需要安装 Visual Studio 2022 社区版，同时也使用 CLion 作为 IDE，
目前已经可以构建出用于特定版本，此处主要支持 Python 3.11.3 版本来构建。当然我们通过修改相关代码，也可以适配不同版本的 Python，
我们在本项目中可以通过 pybind11 用 Python 调用 CPP 程序，也用 CPP 调用 Python 程序，当然需要配置好本地 Python 及环境变量（需要用的虚拟环境的路径即可）

## 性能

- Python-3.11： 59.15727789999801
- Python-3.10： 63.44302169998991
- CPP：52.233

## 环境配置

Windows-11 的 64 位环境中来进行构建，需要依靠 Visual Studio 2022 和 CLion 最新版，在 CLion 中集成 vcpkg 可以方便的构建项目
主要依赖的库为：

- quantlib
- boost
- rapidjson
- pybind11
- talib
- ...

等库，主要实现了将 quantlib 和 boost 打包进入 `.pyd` 文件中，从而可以让 Python 来进行调用，为之后 Python 程序的性能提示做准备。

## 编译

### 提示

本项目由于在 Windows 11 的 64 位操作系统上进行，所以使用 Visual Studio（Microsoft Visual Studio\2022\Community） 的 x86_amd64 架构进行编译
所以我们需要在 CLion 中设置为 Visual Studio 编译。

### 命令

此处可以通过如下命令来进行编译

```
mkdir build
cd build
cmake ..
cmake --build .
```

在 powershell 中建议用如下命令，此处需要设置 `-DCMAKE_TOOLCHAIN_FILE=C:/Users/albert/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake` 来确保使用到该工具链
会找不到 boost 库，因为 boost 库是通过 vcpkg 安装的

```powershell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/Users/albert/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake; cmake --build build
```

## 测试

我们可以在 cpp/tests/test.py 中测试我们编译出来 `.pyd` 文件文件

## 注意事项

注意 Python 版本，我们可以在根目录的 CMakeLists.txt 文件中通过修改 conda 创建的虚拟环境的名字，而设设置不同的版本

## Python 格式化

```shell
pip install -r requirements-dev.txt

配置：https://pre-commit.com/ 设置好 .pre-commit-config.yaml 文件后
pre-commit install
```
