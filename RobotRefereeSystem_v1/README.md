# 简化版机器人裁判系统

## 一、项目说明

本程序完成 C++ 综合考核题：登记四种机器人，处理伤害和补给事件，维护生命值，最后按登记顺序输出状态。
生命值、补给量和状态规则按题目要求实现，演示数据与输出格式和题目给出的示例一致。

## 二、文件结构

```text
RobotRefereeSystem/
├── .vscode/
│   ├── launch.json    // 调试配置
│   ├── settings.json  // 编辑器设置
│   └── tasks.json     // 编译任务
├── CMakeLists.txt     // CMake 编译配置
├── robot.h            // RobotInfo、Team、EventType、MatchEvent、Robot 基类、四个派生类
├── robot.cpp          // 基类的生命值规则，四个派生类的 supply 和 typeName
├── referee.h          // c_RefereeSystem 类声明
├── referee.cpp        // 登记、查找、处理事件、输出状态
├── main.cpp           // 演示数据、事件循环、异常捕获
└── README.md          // 本文件
```

## 三、编译运行

### 方式一：VS Code 一键编译（推荐）

`.vscode` 里已经配好编译任务：

- `tasks.json` 的 `build C++` 任务用 `g++` 编译 `main.cpp`、`robot.cpp`、`referee.cpp`。
- `launch.json` 的 `Debug C++` 会自动调用 `build C++`，然后启动调试。

操作步骤：

1. 用 VS Code 打开 `RobotRefereeSystem` 文件夹（有 `.vscode` 的那一层）。
2. 按 `F5`，或点左侧“运行和调试”，选 **Debug C++**。

### 方式二：命令行编译

在项目根目录执行：

```bash
g++ -std=c++17 -Wall -Wextra -g main.cpp robot.cpp referee.cpp -o main.exe
```

运行：

```bash
./main.exe
```

中文乱码问题已经在代码里处理了：`main.cpp` 的 `main()` 一开始会调用 `setup_console_utf8()`，
把 Windows 控制台的输入/输出代码页切到 UTF-8（65001）。
所以**不需要再手动执行 `chcp 65001`**。

原理：源文件和程序里的字符串是 UTF-8，而 Windows 控制台默认用 GBK（936）解码，两边不一致就会乱码；
统一成 UTF-8 后就能正常显示。用 MSVC 编译时，`CMakeLists.txt` 里的 `/utf-8` 选项保证源码和字符串也按 UTF-8 处理。

### 方式三：CMake

在项目根目录执行：

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

运行（Windows 下可执行文件是 `build/main.exe`）：

```bash
build/main.exe
```

Windows 上 CMake 默认生成器是 NMake，所以上面显式写了 `-G "MinGW Makefiles"`，和项目使用的 g++ 配套。
生成的 `build` 文件夹是编译产物，删掉不影响源码。

## 四、实现的功能

| 要求 | 实现方式 |
| --- | --- |
| 阵营、基础信息、事件 | `Team` 枚举、`RobotInfo` 结构体、`EventType` 枚举、`MatchEvent` 结构体 |
| 四种机器人 | `Robot` 抽象基类，`Infantry`、`Hero`、`Sentry`、`AerialRobot` 四个派生类 |
| 生命值规则 | 初始满血；伤害最低扣到 0；补给不超过最大生命值；退场后不能操作 |
| 补给差异 | 裁判系统只调用 `robot->supply()`，恢复量由派生类决定，没有按类型写分支 |
| 唯一编号 | `registerRobot` 里先用 `findRobot` 查重，重复就抛异常，原机器人保留 |
| 所有权 | `vector<unique_ptr<Robot>>` 保存，`make_unique` 创建，没有手写 `new` / `delete` |
| 异常 | 参数错误用 `invalid_argument`，状态错误用 `logic_error`，主循环捕获 `const exception&` 后继续 |
| 输出 | 事件逐条输出成功或失败原因，最后输出最终状态和成功事件数 |

## 五、题目给定的代码标注

代码中用 `// [题目给定]` 注释标出了题目直接给出的内容，汇总如下：

| 文件 | 标注内容 |
| --- | --- |
| `robot.h` | `Team`、`RobotInfo`、`EventType`、`MatchEvent`；`Robot` 的构造函数、虚析构、`info()`、`hp()`、`maxHp()`、`isAlive()`、`takeDamage()`、纯虚函数 `supply()`、`typeName()` |
| `referee.h` | `registerRobot()`、`findRobot()`、`processEvent()`、`successfulEvents()`、`printStatus()`；`vector<unique_ptr<Robot>>` 存储方式 |
| `main.cpp` | `make_unique` 创建机器人、演示事件数组、`catch (const exception&)` 捕获 |
| `robot.cpp` / `referee.cpp` | 按题目要求抛出的 `invalid_argument` / `logic_error`，以及通过基类指针调用 `supply()` |

除这些之外的部分（宏定义、循环、辅助函数、输出拼接等）都是自己写的，写法保持简单直白：
C 风格的下标循环、`snake_case` 的函数名、宏定义常量、`cout` 输出、中文注释。

## 六、运行结果

```text
=== 登记机器人 ===
1 红方 步兵 步兵 100/100 正常
2 红方 英雄 英雄 150/150 正常
3 蓝方 哨兵 哨兵 120/120 正常
4 蓝方 空中机器人 无人机 80/80 正常
登记失败：机器人编号重复

=== 处理比赛事件 ===
事件 1：机器人 1 受到 35 点伤害 -> 成功，实际扣除 35 点，生命值：65/100
事件 2：机器人 2 受到 50 点伤害 -> 成功，实际扣除 50 点，生命值：100/150
事件 3：机器人 1 接收补给 -> 成功，实际恢复 20 点，生命值：85/100
事件 4：机器人 2 接收补给 -> 成功，实际恢复 30 点，生命值：130/150
事件 5：机器人 4 接收补给 -> 失败：空中机器人不支持地面补给
事件 6：机器人 3 受到 200 点伤害 -> 成功，实际扣除 120 点，生命值：0/120，已退场
事件 7：机器人 3 接收补给 -> 失败：机器人已退场
事件 8：机器人 99 受到 10 点伤害 -> 失败：机器人不存在
事件 9：机器人 1 受到 -5 点伤害 -> 失败：伤害值必须大于 0
事件 10：机器人 1 接收补给 -> 成功，实际恢复 15 点，生命值：100/100
事件 11：机器人 1 接收补给 -> 成功，实际恢复 0 点，生命值：100/100

=== 最终状态 ===
1 红方 步兵 步兵 100/100 正常
2 红方 英雄 英雄 130/150 正常
3 蓝方 哨兵 哨兵 0/120 已退场
4 蓝方 空中机器人 无人机 80/80 正常
成功处理事件数：7
```

## 七、几点说明

1. **名称和演示数据表的差异**
   题目“演示数据”表里 1 号名称是“王八”、3 号名称是“过洞”，但“输出示例”这两行写的是“步兵 步兵”和“哨兵 哨兵”。
   按题目输出示例的列含义（第一个是类型名，第二个是名称），这里按“输出示例”取名，保证运行结果和示例逐字一致。
   如果老师要求用表格里的名字，只改 `main.cpp` 里这两处即可：

   ```cpp
   register_one(make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}));
   register_one(make_unique<Sentry>(RobotInfo{3, "过洞", Team::Blue}));
   ```

2. **基础信息检查放在 `Robot` 构造函数里**
   编号小于等于 0、名称为空、最大生命值小于等于 0 都会抛 `invalid_argument`，所以 `make_unique` 那一步就会失败，`register_one` 捕获后输出原因。

3. **补给失败的顺序**
   `AerialRobot::supply()` 先调用 `requireAlive()`：退场的空中机器人报“机器人已退场”，活着的才报“不支持地面补给”。

4. **事件成功数**
   `success_count` 只在事件真正成功后加 1；登记失败、伤害值非法、对退场机器人操作、空中机器人补给都不会增加。

5. **命名前缀**
   自己定义的类统一加 `c_` 前缀，所以裁判系统类叫 `c_RefereeSystem`；题目点名的 `Robot`、`Infantry`、`Hero`、`Sentry`、`AerialRobot`、`RobotInfo`、`MatchEvent`、`Team`、`EventType` 都保持原名。
   本项目只定义了题目要求的 `RobotInfo` 和 `MatchEvent` 两个结构体，它们保持原名，所以没有加 `st_`。

## 八、已知不足

- 演示数据直接写在 `main.cpp` 里，没有做命令行输入解析。
- 机器人信息只检查了编号、名称、最大生命值，没有检查阵营枚举值是否被强转成非法值。
- 输出格式写死在函数里，如果以后要改格式，需要同时改 `printStatus` 和 `main` 里的拼接。
- 名称按“输出示例”取，和演示数据表不一致，原因见第七节。
