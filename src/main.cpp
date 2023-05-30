#include "option.h"
#include "version.h"
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <ta_common.h>
#include <ta_func.h>
#include <unordered_map>
#include <vector>
namespace py = pybind11;

using namespace std;

int playak() {
  py::scoped_interpreter guard{}; // 初始化 Python 解释器

  std::cout << "coming" << std::endl;
  py::module_ akshare = py::module_::import("akshare");
  py::object version = akshare.attr("__version__");
  std::cout << "Akshare version: " << py::str(version) << std::endl;

  py::object result = akshare.attr("spot_hist_sge")("Au100g");
  //    py::object result = akshare.attr("stock_zh_a_hist")("000001");

  auto columns = result.attr("columns").cast<py::list>();
  auto rows = result.attr("values").cast<py::list>();

  std::vector<std::unordered_map<std::string, py::object>> data;

  for (const auto &row : rows) {
    auto row_tuple = row.cast<py::tuple>();
    std::unordered_map<std::string, py::object> row_data;

    for (size_t i = 0; i < columns.size(); ++i) {
      row_data[columns[i].cast<std::string>()] = row_tuple[i];
    }

    data.push_back(row_data);
  }

  // 打印出数据
  for (const auto &row : data) {
    for (const auto &column : columns) {
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

int cppcallpy() {
  py::scoped_interpreter guard{}; // start the interpreter and keep it alive

  py::print("Hello, World!"); // use the Python API
  return 0;
}

int read() {
  ifstream ifs("C:/Users/albert/CLionProjects/cpp/src/alice.json");
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj); // reader can also read strings
  cout << "Book: " << obj["book"].asString() << endl;
  cout << "Year: " << obj["year"].asUInt() << endl;
  const Json::Value &characters = obj["characters"]; // array of characters
  for (int i = 0; i < characters.size(); i++) {
    cout << "    name: " << characters[i]["name"].asString();
    cout << " chapter: " << characters[i]["chapter"].asUInt();
    cout << endl;
  }
  return 0;
}

/**
 * @brief 测试TA-LIB
 *
 * 该函数接收两个整数作为参数，并返回它们的和。这是一个简单的示例，用于演示如何使用Doxygen风格的注释。
 *
 * @param a 第一个整数
 * @param b 第二个整数
 * @return 两个整数的和
 */
int ta() {
  TA_Integer outBeg2;
  TA_Integer outNbElement2;
  TA_Integer out2[399];

  double open[399] = {
      6395.51, 6388.83, 6391.43, 6393.87, 6451.53,
      6427.35, 6418.26, 6451.03, 6462,

  };
  double high[399] = {
      6397.27, 6394.38, 6411,    6457,    6465.2,  6446.68, 6465,
      6485.33, 6479.74, 6474.19, 6472.69, 6450.56, 6449.09,

  };
  double low[399] = {
      6372, 6370.65, 6385.82, 6392.4, 6428, 6403, 6418.12, 6440, 6441.35,

  };
  double close[399] = {
      6383.76, 6391.42, 6393.96, 6451.07, 6428, 6418.25, 6451.04, 6463.52,

  };
  TA_RetCode retcode2;
  // 初始化TA-LIB，确保只调用一次
  retcode2 = TA_Initialize();
  assert(retcode2 == TA_SUCCESS);
  // 启明星 Morning Star
  // 输出: 1 ~ 100，越大的数值表示越确定该形态
  // retcode2 = TA_CDLMORNINGSTAR(0, 398, open, high, low,close, 0, &outBeg2,
  // &outNbElement2, out2);

  // 黄昏星  输出: -100 ~ -1，越小的数值表示越确定该形态
  retcode2 = TA_CDLEVENINGSTAR(0, 398, open, high, low, close, 0, &outBeg2,
                               &outNbElement2, out2);

  for (int i = 0; i < outNbElement2; i++) {
    std::cout << "time" << outBeg2 + i << "=" << out2[i] << std::endl;
  }

  retcode2 = TA_Shutdown();
  assert(retcode2 == TA_SUCCESS);
  return 0;
}

int main() {
  // 为了显示中文全角字符
  playak();
  cppcallpy();
  read();
  ta();

  cout << "你好，世界！" << PROJECT_VERSION_MAJOR << "."
       << PROJECT_VERSION_MINOR << "." << PROJECT_VERSION_PATCH << endl;
  auto start_time = chrono::high_resolution_clock::now();
  cout << "我要开始计算函数啦" << endl;
  cal_option();
  auto end_time = chrono::high_resolution_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end_time - start_time)
          .count();

  cout << "耗时 " << duration / 1000 << " s" << endl;

  return 0;
}
