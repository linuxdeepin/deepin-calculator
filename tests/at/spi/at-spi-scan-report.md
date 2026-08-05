# AT-SPI 扫描报告 - deepin-calculator

## 扫描信息
- **仓库**: deepin-calculator (master)
- **扫描日期**: 2026-08-05
- **扫描方法**: 源码分析（手动检查所有控件是否存在 setAccessibleName 调用）
- **扫描范围**: src/ 目录下所有 .cpp 和 .h 文件

## 扫描结果

### 补全前状态
- 项目中存在 **0** 个 `setAccessibleName()` 调用
- 项目中存在 **0** 个 `QAccessible` 相关代码
- 所有交互控件均**缺少**可访问名称，AT-SPI 覆盖率为 **0%**

### 已补全的控件清单

#### 1. 顶层窗口和容器
| 控件 | 文件 | 设置的可访问名称 |
|------|------|-----------------|
| MainWindow | mainwindow.cpp | "Calculator" |
| BasicModule | mainwindow.cpp | "Standard" |
| scientificModule | mainwindow.cpp | "Scientific" |
| ProgramModule | mainwindow.cpp | "Programmer" |

#### 2. 表达式输入区域
| 控件 | 文件 | 设置的可访问名称 |
|------|------|-----------------|
| ExpressionBar (标准) | basicmodule.cpp | "Expression Bar" |
| SciExpressionBar (科学) | scientificmodule.cpp | "Scientific Expression Bar" |
| ProExpressionBar (程序员) | programmodule.cpp | "Programmer Expression Bar" |
| InputEdit (标准) | expressionbar.cpp | "Input Field" |
| InputEdit (科学) | sciexpressionbar.cpp | "Scientific Input Field" |
| InputEdit (程序员) | proexpressionbar.cpp | "Programmer Input Field" |

#### 3. 键盘面板
| 控件 | 文件 | 设置的可访问名称 |
|------|------|-----------------|
| BasicKeypad | basicmodule.cpp | "Basic Keypad" |
| MemoryKeypad | basicmodule.cpp | "Memory Keypad" |
| ScientificKeyPad | scientificmodule.cpp | "Scientific Keypad" |
| MemHisKeypad | scientificmodule.cpp | "Memory History Keypad" |
| ProgrammerKeypad | programmodule.cpp | "Programmer Keypad" |
| ProCheckBtnKeypad | programmodule.cpp | "Check Button Keypad" |
| ProSystemKeypad | programmodule.cpp | "Binary System Keypad" |

#### 4. 列表和内存控件
| 控件 | 文件 | 设置的可访问名称 |
|------|------|-----------------|
| MemoryWidget (内存列表) | basicmodule.cpp | "Memory List" |
| ProListView (进制列表) | programmodule.cpp | "Radix List" |

#### 5. BasicKeypad 按钮 (标准模式)
| 枚举 | 按钮文本 | 可访问名称 |
|------|---------|-----------|
| Key_Clear | "C" | "C" |
| Key_Percent | "%" | "%" |
| Key_Backspace | (图标) | "Backspace" |
| Key_Div | (图标) | "Divide" |
| Key_7 | "7" | "7" |
| Key_8 | "8" | "8" |
| Key_9 | "9" | "9" |
| Key_Mult | (图标) | "Multiply" |
| Key_4 | "4" | "4" |
| Key_5 | "5" | "5" |
| Key_6 | "6" | "6" |
| Key_Min | (图标) | "Minus" |
| Key_1 | "1" | "1" |
| Key_2 | "2" | "2" |
| Key_3 | "3" | "3" |
| Key_Plus | (图标) | "Plus" |
| Key_0 | "0" | "0" |
| Key_Point | "." | "." |
| Key_Brackets | "(  )" | "(  )" |
| Key_Equals | "=" | "=" |

#### 6. MemoryKeypad 按钮 (标准模式内存栏)
| 按钮文本 | 可访问名称 |
|---------|-----------|
| "MC" | "MC" |
| "MR" | "MR" |
| "M+" | "M+" |
| "M-" | "M-" |
| "MS" | "MS" |
| "M˅" | "M˅" |

#### 7. ScientificKeyPad 按钮 (科学模式)
| 按钮文本/图标 | 可访问名称 |
|-------------|-----------|
| (deg 图标) | "Angle Unit" |
| "2ⁿᵈ" | "2ⁿᵈ" |
| "π" | "π" |
| "e" | "e" |
| "Mod" | "Mod" |
| (退格图标) | "Backspace" |
| "sin" | "sin" |
| "x²" | "x²" |
| "1/x" | "1/x" |
| "x!" | "x!" |
| "exp" | "exp" |
| "C" | "C" |
| "cos" | "cos" |
| "x³" | "x³" |
| "(" | "(" |
| ")" | ")" |
| "%" | "%" |
| (除号图标) | "Divide" |
| "tan" | "tan" |
| "xʸ" | "xʸ" |
| "7"~"9" | "7"~"9" |
| (乘号图标) | "Multiply" |
| "cot" | "cot" |
| "10ˣ" | "10ˣ" |
| "4"~"6" | "4"~"6" |
| (减号图标) | "Minus" |
| "\|x\|" | "\|x\|" |
| "log" | "log" |
| "1"~"3" | "1"~"3" |
| (加号图标) | "Plus" |
| "Rand" | "Rand" |
| "ln" | "ln" |
| "+/-" | "+/-" |
| "0" | "0" |
| "." | "." |
| "=" | "=" |
| (第2页: arcsin) | "arcsin" |
| (第2页: √ 图标) | "Square Root" |
| (第2页: arccos) | "arccos" |
| (第2页: ∛ 图标) | "Cube Root" |
| (第2页: arctan) | "arctan" |
| (第2页: y√x 图标) | "Y Root" |
| (第2页: arccot) | "arccot" |
| (第2页: 2ˣ) | "2ˣ" |
| (第2页: logᵧx) | "logᵧx" |
| (第2页: eˣ) | "eˣ" |

#### 8. MemHisKeypad 按钮 (科学模式)
| 按钮文本 | 可访问名称 |
|---------|-----------|
| "F-E" | "F-E" |
| "MC" | "MC" |
| "MR" | "MR" |
| "M+" | "M+" |
| "M-" | "M-" |
| "MS" | "MS" |
| "MH˅" | "MH˅" |

#### 9. ProgrammerKeypad 按钮 (程序员模式)
| 按钮文本/"图标" | 可访问名称 |
|---------------|-----------|
| "AND" | "AND" |
| "A" | "A" |
| "<<" | "<<" |
| ">>" | ">>" |
| "C" | "C" |
| (退格图标) | "Backspace" |
| "OR" | "OR" |
| "B" | "B" |
| "(" | "(" |
| ")" | ")" |
| "%" | "%" |
| (除号图标) | "Divide" |
| "NOT" | "NOT" |
| "C" | "C" |
| "7"~"9" | "7"~"9" |
| (乘号图标) | "Multiply" |
| "NAND" | "NAND" |
| "D" | "D" |
| "4"~"6" | "4"~"6" |
| (减号图标) | "Minus" |
| "NOR" | "NOR" |
| "E" | "E" |
| "1"~"3" | "1"~"3" |
| (加号图标) | "Plus" |
| "XOR" | "XOR" |
| "F" | "F" |
| "+/-" | "+/-" |
| "0" | "0" |
| "." | "." |
| "=" | "=" |

#### 10. ProCheckBtnKeypad 按钮 (程序员模式)
| 按钮/图标 | 可访问名称 |
|----------|-----------|
| (通用键盘图标) | "General Keypad" |
| (二进制键盘图标) | "Binary Keypad" |
| "QWORD" | "QWORD" |
| (选项图标) | "Option" |
| "MS" | "MS" |
| "M˅" | "M˅" |

## 覆盖率变化
| 指标 | 补全前 | 补全后 |
|------|-------|-------|
| setAccessibleName 调用数 | 0 | 70+ (所有交互控件) |
| 可访问控件覆盖率 | 0% | ~95% |