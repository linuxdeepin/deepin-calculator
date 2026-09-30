#!/bin/bash

# SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

builddir=build-test
reportdir=build-ut
# 本脚本所在目录（tests/），供调用同目录下的 gen-ut-summary.py（须在 cd 前解析）
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
rm -r $builddir 2>/dev/null || true
rm -r ../$builddir 2>/dev/null || true
rm -r $reportdir 2>/dev/null || true
rm -r ../$reportdir 2>/dev/null || true
mkdir ../$builddir 2>/dev/null || true
mkdir ../$reportdir 2>/dev/null || true
cd ../$builddir || exit 1

# 编译
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_SAFETYTEST_ARG="CMAKE_SAFETYTEST_ARG_ON" ..
make -j8

# 创建报告目录
mkdir -p report

# 运行测试并生成 XML 结果
./tests/deepin-calculator-test --gtest_output=xml:./report/report_deepin-calculator.xml

workdir=$(cd ../$(dirname $0)/$builddir; pwd)

# 统计代码覆盖率并生成 HTML 报告
lcov -d $workdir -c -o ./coverage.info

lcov --extract ./coverage.info '*/src/*' -o ./coverage.info

lcov --remove ./coverage.info '*/tests/*' -o ./coverage.info

genhtml -o ./html ./coverage.info

mv ./html/index.html ./html/cov_deepin-calculator.html

# 收集 ASAN、UT、代码覆盖率结果至指定文件夹
cp -r html ../$reportdir/ 2>/dev/null || true
cp -r report ../$reportdir/ 2>/dev/null || true
cp asan*.log* ../$reportdir/asan_deepin-calculator.log 2>/dev/null || true

# 生成 ut-summary.json（测试 + 覆盖率格式化汇总，含 per-suite / per-file 明细）
# 汇总逻辑独立在 tests/gen-ut-summary.py，本脚本仅传参调用
report_dir_abs=$(cd ../$reportdir && pwd)
project_root=$(cd "$workdir/.." && pwd)
if command -v python3 >/dev/null 2>&1; then
    python3 "$SCRIPT_DIR/gen-ut-summary.py" \
        --xml-dir "$workdir/report" \
        --coverage-info "$workdir/coverage.info" \
        --project-root "$project_root" \
        --output "$report_dir_abs/ut-summary.json"
else
    echo "[警告] 未检测到 python3，跳过 ut-summary.json 生成"
fi

if [ -s "$report_dir_abs/ut-summary.json" ]; then
    echo "ut-summary.json 已生成: $report_dir_abs/ut-summary.json"
else
    echo "[警告] ut-summary.json 未生成（请检查上方 python3 输出）"
fi

echo "测试完成！报告已生成到: ../$reportdir （含 ut-summary.json）"

exit 0
