// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_expressionbar.h"

#include <QClipboard>

#include "../../src/widgets/expressionbar.h"
#include "../../3rdparty/core/settings.h"
#include "../stub.h"

// 固定系统区域设置相关的环境依赖：分组开启、小数点"."、分组符","，
// 与本文件用例断言中硬编码的格式一致
static bool stub_grouping_on() { return true; }
static QString stub_dec_symbol() { return "."; }
static QString stub_grp_symbol() { return ","; }

Ut_ExpressionBar::Ut_ExpressionBar()
{
}

TEST_F(Ut_ExpressionBar, mouseMoveEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    QMouseEvent *m = new QMouseEvent(QMouseEvent::Type::MouseMove, m_expressionBar->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
    m_expressionBar->mouseMoveEvent(m);
    delete m;
    //无ASSERT
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterNumberEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_inputNumber = true;
    m_expressionBar->m_hisRevision = -1;
    m_expressionBar->enterNumberEvent("1");
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1");
    m_expressionBar->m_isResult = true;
    m_expressionBar->enterNumberEvent("1");
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1");
    m_expressionBar->m_isContinue = false;
    m_expressionBar->enterNumberEvent("1");
    m_expressionBar->enterNumberEvent("1");
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "11");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterSymbolEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->m_inputEdit->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterSymbolEvent("-");
    EXPECT_EQ(m_expressionBar->m_inputEdit->text(), "3－");
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterSymbolEvent("＋");
    EXPECT_EQ(m_expressionBar->m_inputEdit->text(), "3＋");
    m_expressionBar->m_inputEdit->setCursorPosition(0);
    m_expressionBar->enterSymbolEvent("-");
    EXPECT_EQ(m_expressionBar->m_inputEdit->text(), "－3＋");
    m_expressionBar->m_inputEdit->setCursorPosition(1);
    m_expressionBar->enterSymbolEvent("-");
    EXPECT_EQ(m_expressionBar->m_inputEdit->text(), "－3＋");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterPointEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->enterPointEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "0.");
    m_expressionBar->findChild<InputEdit *>()->setText("1");
    m_expressionBar->enterPointEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1.");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterPointEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋0.1");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterBackspaceEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    Stub stub;
    stub.set(ADDR(Settings, getSystemDigitGrouping), stub_grouping_on);
    stub.set(ADDR(Settings, getSystemDecimalSymbol), stub_dec_symbol);
    stub.set(ADDR(Settings, getSystemDigitGroupingSymbol), stub_grp_symbol);
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->allElection();
    m_expressionBar->enterBackspaceEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    m_expressionBar->findChild<InputEdit *>()->setText("1.＋1");
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->enterBackspaceEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1");
    m_expressionBar->findChild<InputEdit *>()->setText("111,111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(4);
    m_expressionBar->enterBackspaceEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "11,111");
    m_expressionBar->findChild<InputEdit *>()->setText("1,111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(3);
    m_expressionBar->enterBackspaceEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "111");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterClearEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_isAllClear = false;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterClearEvent();
    m_expressionBar->m_isAllClear = true;
    m_expressionBar->enterClearEvent();
    m_expressionBar->m_isAllClear = false;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterClearEvent();
    EXPECT_EQ(m_expressionBar->m_listModel->rowCount(QModelIndex()), 0);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterEqualEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋");
    historicalLinkageIndex his;
    his.linkageTerm = 0;
    his.linkageValue = QString::number(3);
    his.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his);
    his.linkageTerm = 1;
    his.linkageValue = QString::number(3);
    his.linkedItem = 2;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->m_hisRevision = -1;
    m_expressionBar->m_isLinked = true;
    m_expressionBar->enterEqualEvent();

    m_expressionBar->findChild<InputEdit *>()->setText("1＋");
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->m_hisRevision = 1;
    m_expressionBar->enterEqualEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋");

    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->m_hisRevision = -1;
    m_expressionBar->m_isLinked = true;
    m_expressionBar->enterEqualEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3");

    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->m_hisRevision = 0;
    m_expressionBar->m_isLinked = true;
    m_expressionBar->m_Selected = 0;
    m_expressionBar->enterEqualEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3");

    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterEqualEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "2");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterPercentEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->enterPercentEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterPercentEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋1%");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterPercentEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1%＋1%");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterBracketsEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterBracketsEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3()");
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterBracketsEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3(())");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterLeftBracketsEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterLeftBracketsEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3(");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterLeftBracketsEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "(3(");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, enterRightBracketsEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterRightBracketsEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3)");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterRightBracketsEvent();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), ")3)");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, moveLeft)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->moveLeft();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 2);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, moveRight)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->moveRight();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 1);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, copyResultToClipboard)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->copyResultToClipboard();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->copyResultToClipboard();
    EXPECT_EQ(QApplication::clipboard()->text(), "1＋2");
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->copyClipboard2Result();
    EXPECT_EQ(QApplication::clipboard()->text(), "1＋2");
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->copyClipboard2Result();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "31＋21＋2");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, allElection)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->getSelection().selected, "1＋2");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, shear)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->shear();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    EXPECT_FALSE(m_expressionBar->m_isResult);
    EXPECT_TRUE(m_expressionBar->m_isContinue);
    EXPECT_FALSE(m_expressionBar->m_isUndo);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, deleteText)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->deleteText();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    EXPECT_FALSE(m_expressionBar->m_isResult);
    EXPECT_TRUE(m_expressionBar->m_isContinue);
    EXPECT_FALSE(m_expressionBar->m_isUndo);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, clearLinkageCache)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    historicalLinkageIndex his;
    his.linkageTerm = 0;
    his.linkageValue = QString::number(3);
    his.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->clearLinkageCache("1＋2＝3", true);
    historicalLinkageIndex his1;
    his1.linkageTerm = 0;
    his1.linkageValue = QString::number(3);
    his1.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his1);
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->clearLinkageCache("1＋2＝3", false);
    //    m_expressionBar->shear();
    EXPECT_EQ(m_expressionBar->m_hisLink.count(), 0);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, settingLinkage)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    historicalLinkageIndex his;
    his.linkageTerm = 0;
    his.linkageValue = QString::number(3);
    his.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->m_hisRevision = 0;
    m_expressionBar->settingLinkage();
    EXPECT_EQ(m_expressionBar->m_hisLink.count(), 1);
    m_expressionBar->m_hisLink.clear();
    his.linkageTerm = 1;
    his.linkageValue = QString::number(3);
    his.linkedItem = 0;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->m_hisRevision = 0;
    m_expressionBar->settingLinkage();
    // m_meanexp 构造时即为 true（0446584），纯数字表达式默认保留联动记录，
    // settingLinkage 走 push_back 而非 pop_back，联动项数量变为 2
    EXPECT_EQ(m_expressionBar->m_hisLink.count(), 2);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, setResultFalse)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_isResult = true;
    m_expressionBar->setResultFalse();
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, replaceSelection)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->replaceSelection("1＋2");
    EXPECT_FALSE(m_expressionBar->m_isResult);
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, revisionResults)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->revisionResults(m_expressionBar->m_listModel->index(0, 0));
    EXPECT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋2");
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, cancelLink)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    historicalLinkageIndex his;
    his.linkageTerm = 0;
    his.linkageValue = QString::number(3);
    his.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->m_hisRevision = 1;
    EXPECT_TRUE(m_expressionBar->cancelLink(1));
    delete m_expressionBar;
}

TEST_F(Ut_ExpressionBar, pointFaultTolerance)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    EXPECT_EQ(m_expressionBar->pointFaultTolerance("123321.123321。"), "123321.123321");
    EXPECT_EQ(m_expressionBar->pointFaultTolerance("123321。123321."), "123321123321.");
    EXPECT_EQ(m_expressionBar->pointFaultTolerance(".123321"), "0.123321");
    delete m_expressionBar;
}
