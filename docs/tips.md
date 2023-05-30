## 输出中文

在 windows 11 中使用 CLion 并且使用 MSVC 编译，想要输出全角中文字符，主要为 CPP11，该如何设置：

```c++
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

int main() {
  // 创建一个 UTF-8 编码的字符串，其中包含全角中文字符
  std::string fullwidth_chinese_text = u8"我的特殊符号啊！！！！：：：：：";

  // 使用 C++11 的 codecvt_utf8_utf16 类在 UTF-8 和 UTF-16 之间转换字符串
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::wstring wide_fullwidth_chinese_text = converter.from_bytes(fullwidth_chinese_text);

  // 使用 std::wcout 输出全角中文字符串
  std::wcout.imbue(std::locale("chs"));
  std::wcout << wide_fullwidth_chinese_text << std::endl;

  return 0;
}
```

以上是一个案例，仅供参考

## CPP 中调用 AKShare

在 CPP 20 中调用 AKShare 来获取数据，根据本文档搭建 pybind 11 环境

```c++
#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <unordered_map>
#include <vector>

namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{};// 初始化 Python 解释器

    std::cout << "coming" << std::endl;
    py::module_ akshare = py::module_::import("akshare");
    py::object version = akshare.attr("__version__");
    std::cout << "Akshare version: " << py::str(version) << std::endl;

    py::object result = akshare.attr("spot_hist_sge")("Au100g");
    //    py::object result = akshare.attr("stock_zh_a_hist")("000001");


    auto columns = result.attr("columns").cast<py::list>();
    auto rows = result.attr("values").cast<py::list>();

    std::vector<std::unordered_map<std::string, py::object>> data;

    for (const auto &row: rows) {
        auto row_tuple = row.cast<py::tuple>();
        std::unordered_map<std::string, py::object> row_data;

        for (size_t i = 0; i < columns.size(); ++i) {
            row_data[columns[i].cast<std::string>()] = row_tuple[i];
        }

        data.push_back(row_data);
    }

    
    // 打印出数据
    for (const auto &row: data) {
        for (const auto &column: columns) {
            auto column_name = column.cast<std::string>();
            std::cout << column_name << ": " << py::str(row.at(column_name)) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "-------------------------------------" << std::endl;

    // 取具体的值
    std::cout << data[1]["date"] << " ";
    std::cout << data[1]["open"] << " ";

    return 0;
}
```