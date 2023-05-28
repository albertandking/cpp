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
