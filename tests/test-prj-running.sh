#!/bin/bash

# SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

builddir=build-test
reportdir=build-ut
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

echo "测试完成！报告已生成到: ../$reportdir"

exit 0
