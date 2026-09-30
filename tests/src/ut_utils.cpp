// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_utils.h"

#include "../../src/utils.h"
#include "../../3rdparty/core/settings.h"
#include "stub.h"

Ut_Utils::Ut_Utils()
{

}

// 固定系统区域设置相关的环境依赖，使测试结果与运行环境解耦：
// 分组开启、小数点"."、分组符","，与用例断言中硬编码的格式一致
static bool stub_grouping_on() { return true; }
static QString stub_dec_symbol() { return "."; }
static QString stub_grp_symbol() { return ","; }

QString stub_filePath_utils(const QString &fileName)
{
    Q_UNUSED(fileName);
    return QByteArray("111");
}

TEST_F(Ut_Utils, getwidget)
{
    Utils *m_utils = new Utils;
    Stub stub;
    stub.set(ADDR(QDir, filePath), stub_filePath_utils);
    ASSERT_EQ(m_utils->getConfigDir(), "111");
    delete m_utils;
}

QByteArray stub_readall_utils()
{
    return QByteArray("111");
}

TEST_F(Ut_Utils, getQssContent)
{
    Utils *m_utils = new Utils;
    Stub stub;
    stub.set(ADDR(QFile, readAll), stub_readall_utils);
    m_utils->getQssContent("/etc/apt/sources.list");
    ASSERT_EQ(m_utils->getQssContent("/etc/apt/sources.list"), "111");
    delete m_utils;
}

TEST_F(Ut_Utils, formatThousandsSeparators)
{
    Utils *m_utils = new Utils;
    Stub stub;
    stub.set(ADDR(Settings, getSystemDigitGrouping), stub_grouping_on);
    stub.set(ADDR(Settings, getSystemDecimalSymbol), stub_dec_symbol);
    stub.set(ADDR(Settings, getSystemDigitGroupingSymbol), stub_grp_symbol);
    m_utils->formatThousandsSeparators("1234");
    EXPECT_EQ(m_utils->formatThousandsSeparators("1234"), "1,234");
    delete m_utils;
}
