#include "option.h"
#include "version.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main() {
  cout << "Project version: " << PROJECT_VERSION_MAJOR << "."
       << PROJECT_VERSION_MINOR << "." << PROJECT_VERSION_PATCH << endl;
  auto start_time = std::chrono::high_resolution_clock::now();
  string f = "我是中文！！！！！！！！！！！！！！";
  cout << f << endl;

  int a = cal_option();
  cout << "I am main file" << endl;
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      end_time - start_time)
                      .count();

  // 输出结果
  std::cout << "Elapsed time: " << duration << " milliseconds" << std::endl;
  system("pause");
  return 0;
}
