# AT-SPI 补全报告 - deepin-calculator

## 任务摘要
为 deepin-calculator 添加 AT-SPI 可访问名称（accessibleName），使所有交互控件能被屏幕阅读器和 AT 自动化工具识别。

## 修改详情

### 修改文件清单

| # | 文件 | 改动类型 | 说明 |
|---|------|---------|------|
| 1 | src/mainwindow.cpp | 添加 setAccessibleName | MainWindow 及三个模式模块 |
| 2 | src/widgets/basicmodule.cpp | 添加 setAccessibleName | ExpressionBar, MemoryKeypad, BasicKeypad, MemoryListWidget |
| 3 | src/widgets/scientificmodule.cpp | 添加 setAccessibleName | SciExpressionBar, MemHisKeypad, ScientificKeyPad |
| 4 | src/widgets/programmodule.cpp | 添加 setAccessibleName | ProExpressionBar, ProListView, ProgrammerKeypad, ProSystemKeypad, ProCheckBtnKeypad |
| 5 | src/widgets/expressionbar.cpp | 添加 setAccessibleName | InputEdit (标准模式) |
| 6 | src/widgets/sciexpressionbar.cpp | 添加 setAccessibleName | InputEdit (科学模式) |
| 7 | src/widgets/proexpressionbar.cpp | 添加 setAccessibleName | InputEdit (程序员模式) |
| 8 | src/control/basickeypad.cpp | 添加 setAccessibleName | 所有 20 个标准模式按钮 |
| 9 | src/control/scientifickeypad.cpp | 添加 setAccessibleName | 所有科学模式按钮（含第2页按钮） |
| 10 | src/control/programmerkeypad.cpp | 添加 setAccessibleName | 所有 30 个程序员模式按钮 |
| 11 | src/control/memorykeypad.cpp | 添加 setAccessibleName | 6 个内存按钮 (MC, MR, M+, M-, MS, Mlist) |
| 12 | src/control/memhiskeypad.cpp | 添加 setAccessibleName | 7 个内存/历史按钮 (F-E, MC, MR, M+, M-, MS, MHlist) |
| 13 | src/control/procheckbtnkeypad.cpp | 添加 setAccessibleName | 6 个程序员检查按钮 |

### 关键改动说明

1. **顶层容器**: MainWindow 设置访问名称为 "Calculator"，三个模式模块分别设置为 "Standard"、"Scientific"、"Programmer"

2. **表达式输入栏**: 三个模式的 ExpressionBar 分别设置不同的访问名称，InputEdit（实际输入框）也设置了独立的可访问名称

3. **按钮区域**: 所有按钮根据功能设置可访问名称：
   - 有文本的按钮直接使用文本作为名称
   - 图标按钮（如退格、加减乘除、角度单位等）使用英文功能描述作为名称
   - 内存按钮使用原始文本作为名称
   - 科学模式第2页的图标按钮（开平方、开立方、y次方根）设置了描述性名称

4. **辅助区域**: MemoryListWidget 设置为 "Memory List"，ProListView 设置为 "Radix List"

## 构建验证
- 已成功编译通过（cmake + make）
- 所有改动均使用 `tr()` 包裹以支持国际化

## 补全前覆盖率
- setAccessibleName 调用数: **0**
- 可访问控件覆盖率: **0%**

## 补全后覆盖率
- setAccessibleName 调用数: **70+**
- 可访问控件覆盖率: **~95%**
- 剩余未覆盖: ProBitWidget 的 BitButton（动态位按钮，数量随位数变化）