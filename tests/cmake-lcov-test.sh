#!/bin/bash

# SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

utdir=build-test
rm -r $utdir 2>/dev/null || true
rm -r ../$utdir 2>/dev/null || true
mkdir ../$utdir 2>/dev/null || true
cd ../$utdir || exit 1

cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8

mkdir -p report

./tests/deepin-calculator-test --gtest_output=xml:./report/report.xml

workdir=$(cd ../$(dirname $0)/$utdir; pwd)

lcov -d $workdir -c -o ./report/coverage.info

lcov --extract ./report/coverage.info '*/src/*' -o ./report/coverage.info

lcov --remove ./report/coverage.info '*/tests/*' -o ./report/coverage.info

genhtml -o ./report ./report/coverage.info

echo "测试完成！报告已生成到: ./report"

exit 0
