// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_simplelistmodel.h"
#include <QClipboard>
#include "../../3rdparty/core/settings.h"
#include "../stub.h"

// 固定系统区域设置相关的环境依赖：分组开启、小数点"."、分组符","，
// 与本文件用例断言中硬编码的格式一致（SimpleListModel 路径无非法字符过滤器，
// 16/8/2 进制的空格分组由 formatThousandsSeparatorsPro 直接呈现）
static bool stub_grouping_on() { return true; }
static QString stub_dec_symbol() { return "."; }
static QString stub_grp_symbol() { return ","; }

Ut_SimpleListModel::Ut_SimpleListModel()
{

}

void Ut_SimpleListModel::SetUp()
{
    m_simpleListModel = new SimpleListModel();
}

void Ut_SimpleListModel::TearDown()
{
    delete m_simpleListModel;
}

TEST_F(Ut_SimpleListModel, data)
{
    m_simpleListModel->appendText("1", false);
    EXPECT_EQ(m_simpleListModel->data(m_simpleListModel->index(0, 0), SimpleListModel::Role::ExpressionWithOutTip), "1");
    m_simpleListModel->appendText("2", false);
    EXPECT_EQ(m_simpleListModel->data(m_simpleListModel->index(1, 0), SimpleListModel::Role::ExpressionCount), 2);
    m_simpleListModel->appendText("3", false);
    EXPECT_EQ(m_simpleListModel->data(m_simpleListModel->index(1, 0), SimpleListModel::Role::ExpressionPrevious), "1");
    m_simpleListModel->data(m_simpleListModel->index(1, 0), 7);
    EXPECT_EQ(m_simpleListModel->data(m_simpleListModel->index(1, 0), SimpleListModel::Role::ExpressionNext), "3");
}

TEST_F(Ut_SimpleListModel, data1)
{
    m_simpleListModel->appendText("1", true);
    m_simpleListModel->appendText("2", true);
    m_simpleListModel->appendText("3", true);
    EXPECT_EQ(m_simpleListModel->data(m_simpleListModel->index(0, 0), SimpleListModel::Role::ExpressionCount), 1);
    EXPECT_EQ(m_simpleListModel->data(m_simpleListModel->index(1, 0), SimpleListModel::Role::ExpressionNext), "3");
}

TEST_F(Ut_SimpleListModel, updataList1)
{
    for (int i = 0; i < 500; i++) {
        m_simpleListModel->m_expressionList.append("1");
    }
    m_simpleListModel->updataList("2", 500, true);
    m_simpleListModel->updataList("3", 500, false);
    m_simpleListModel->updataList("3x3", 500, false);
    // sci=true: delete(499)+insert=500; sci=false: delete(0)+replace=499; sci=false: replace=499
    EXPECT_EQ(m_simpleListModel->m_expressionList.count(), 499);
}

TEST_F(Ut_SimpleListModel, updataList2)
{
    for (int i = 0; i < 500; i++) {
        m_simpleListModel->m_expressionList.append("1");
        m_simpleListModel->m_answerlist.append(Quantity(1));
    }
    m_simpleListModel->updataList(Quantity(9), "3x3", 500);
    EXPECT_EQ(m_simpleListModel->m_expressionList.count(), 500);
}

TEST_F(Ut_SimpleListModel, deleteItem)
{
    m_simpleListModel->m_expressionList.append("1");
    m_simpleListModel->m_answerlist.append(Quantity(1));
    m_simpleListModel->deleteItem(0);
    EXPECT_EQ(m_simpleListModel->m_expressionList.count(), 0);
}

TEST_F(Ut_SimpleListModel, copyToClipboard)
{
    m_simpleListModel->m_expressionList.append("1");
    m_simpleListModel->copyToClipboard(0);
    EXPECT_EQ(QApplication::clipboard()->text(), "1");
}

TEST_F(Ut_SimpleListModel, getAnswer)
{
    m_simpleListModel->m_answerlist.append(Quantity(1));
    m_simpleListModel->getAnswer(1);
    EXPECT_EQ(m_simpleListModel->getAnswer(0), Quantity(1));
}

TEST_F(Ut_SimpleListModel, radixChanged)
{
    Stub stub;
    stub.set(ADDR(Settings, getSystemDigitGrouping), stub_grouping_on);
    stub.set(ADDR(Settings, getSystemDecimalSymbol), stub_dec_symbol);
    stub.set(ADDR(Settings, getSystemDigitGroupingSymbol), stub_grp_symbol);
    m_simpleListModel->m_expressionList.append("123＋321 ＝ 444");
    m_simpleListModel->radixChanged(16, 10);
    EXPECT_EQ(m_simpleListModel->m_expressionList.at(0), "291＋801＝1,092");
    m_simpleListModel->radixChanged(10, 8);
    EXPECT_EQ(m_simpleListModel->m_expressionList.at(0), "443＋1 441＝2 104");
    m_simpleListModel->radixChanged(8, 2);
    EXPECT_EQ(m_simpleListModel->m_expressionList.at(0), "1 0010 0011＋11 0010 0001＝100 0100 0100");
    m_simpleListModel->radixChanged(2, 16);
    EXPECT_EQ(m_simpleListModel->m_expressionList.at(0), "123＋321＝444");
}

TEST_F(Ut_SimpleListModel, radixChanged1)
{
    m_simpleListModel->m_expressionList.append("－123and321 ＝ 444");
    m_simpleListModel->radixChanged(10, 2);
    EXPECT_EQ(m_simpleListModel->m_expressionList.at(0),
              "1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0101 and 1 0100 0001＝1 1011 1100");
}

TEST_F(Ut_SimpleListModel, formatExpression)
{
    m_simpleListModel->formatExpression(10, "321");
    m_simpleListModel->formatExpression(8, "321");
    m_simpleListModel->formatExpression(2, "321");
    QString str = m_simpleListModel->formatExpression(16, "321");
    EXPECT_EQ(str, "0x321");
}

TEST_F(Ut_SimpleListModel, answerOutOfRange)
{
    m_simpleListModel->m_expressionList.append("123＋321 ＝ 444");
    m_simpleListModel->answerOutOfRange(Quantity(123));
    EXPECT_EQ(m_simpleListModel->m_expressionList.at(0), "123＋321 ＝123");
}
