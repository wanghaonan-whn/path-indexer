# path-indexer

基于目录/文件命名规则的数据检索工具，用 C++ 编写。

面向"数据分散在服务器目录里，靠文件夹名（路局、车型、车号、站点等）编码信息"的场景。目标是把非结构化的路径名解析成结构化字段，建立本地索引，实现比手动翻目录、等平台查询更快的组合条件检索。

## 背景

数据大多存放在类似这样的路径下：

```
/data/北京局/CRH380A/1234/北京南站/
```

或者文件名本身编码信息：

```
C1_1234_0001.jpg
```

命名规则在不同批次、不同来源之间可能不完全一致，因此解析逻辑需要可配置、且能优雅处理解析失败，而不是让脏数据搞崩整个扫描流程。

## 功能规划

- [x] **PathParser** —— 核心路径解析类，支持两种模式：
  - 按分隔符切分 + 字段位置映射（`_` / `/` / `-` 等）
  - 按正则表达式提取（`std::regex`），支持中文等任意字符
  - 解析失败时返回 `success = false`，不抛异常
- [ ] **DataRecord** —— 单条数据记录，包含路局、车型、车号、站点、完整路径、解析成功标志；重载 `operator<<`（调试打印）和 `operator==`（去重）
- [ ] **DirectoryScanner** —— 遍历服务器目录，对每个子路径调用 `PathParser`，产出 `vector<DataRecord>`
- [ ] **IndexBuilder** —— 对车型、路局等字段建 `unordered_map<string, vector<int>>` 倒排索引，实现 O(1) 查询
- [ ] **QueryEngine** —— 支持多条件组合查询（如"路局=北京局 AND 车型=CRH380A"），中文字段支持子串模糊匹配
- [ ] **导出与缓存** —— 索引结果落盘为 CSV/JSON，支持按修改时间做增量更新，避免重复扫描服务器

## 项目结构

```
path-indexer/
├── include/
│   └── PathParser.h
├── src/
│   └── PathParser.cpp
├── tests/
├── CMakeLists.txt
└── README.md
```

## 环境要求

- C++17 及以上
- CMake 3.10+
- GCC / Clang（Windows 下 MSVC 也可）

## 快速开始

```bash
mkdir build && cd build
cmake ..
make
```

## 使用示例

```cpp
#include "PathParser.h"

// 按分隔符解析
auto p1 = PathParser::byDelimiter('_', {"channel", "train_num", "index"});
auto r1 = p1.parse("C1_1234_0001.jpg");
if (r1.success) {
    std::cout << r1.fields["train_num"] << std::endl;
}

// 按正则解析
auto p2 = PathParser::byRegex(R"((\w+)_(\d+)_(\d+))", {"channel", "train_num", "index"});
auto r2 = p2.parse("C1_1234_0001.jpg");
```

## 路线图

当前处于第一阶段（`PathParser` 核心解析类）。后续按 DataRecord → DirectoryScanner + IndexBuilder → QueryEngine → 导出/缓存 的顺序推进。

## License

MIT
