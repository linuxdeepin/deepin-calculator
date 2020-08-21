#include "ut_sciexpressionbar.h"
#define private public
#include "src/modules/sciexpressionbar.h"
#undef private

Ut_SciexpressionBar::Ut_SciexpressionBar()
{

}

TEST_F(Ut_SciexpressionBar, mouseMoveEvent)
{
    SciExpressionBar *m_sciexpressionBar = new SciExpressionBar;
    m_sciexpressionBar->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_sciexpressionBar->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    //无ASSERT
    DSettings::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, getanswer)
{
    SciExpressionBar *m_sciexpressionBar = new SciExpressionBar;
    m_sciexpressionBar->m_pair.first = false;
    ASSERT_TRUE(m_sciexpressionBar->getanswer().isZero());
    //无ASSERT
    DSettings::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterNumberEvent)
{
    SciExpressionBar *m_sciexpressionBar = new SciExpressionBar;
    m_sciexpressionBar->m_inputNumber = true;
    m_sciexpressionBar->m_isResult = true;
    m_sciexpressionBar->m_isContinue = false;
    m_sciexpressionBar->enterNumberEvent("1");
    m_sciexpressionBar->m_inputNumber = false;
    m_sciexpressionBar->m_isResult = true;
    m_sciexpressionBar->enterNumberEvent("1");
    m_sciexpressionBar->m_inputEdit->setText("π");
    m_sciexpressionBar->enterNumberEvent("1");
    ASSERT_EQ(m_sciexpressionBar->m_inputEdit->text(), "π×1");
    DSettings::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterSymbolEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
//    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
//    m_expressionBar->findChild<InputEdit *>()->setText("3");
//    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterSymbolEvent("＋");
    m_expressionBar->findChild<InputEdit *>()->clear();
    m_expressionBar->enterSymbolEvent("-");
    m_expressionBar->enterSymbolEvent("-");
    m_expressionBar->enterSymbolEvent("＋");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterSymbolEvent("-");
    m_expressionBar->findChild<InputEdit *>()->setText("1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterSymbolEvent("-");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterSymbolEvent("-");
    m_expressionBar->findChild<InputEdit *>()->setText("11");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterSymbolEvent("-");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1－1");
    DSettings::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterPercentEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterPercentEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterPercentEvent();
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterPercentEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterPercentEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterPercentEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1%111");
    DSettings::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterPointEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterPointEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1");
    m_expressionBar->enterPointEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterPointEvent();
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(4);
    m_expressionBar->enterPointEvent();
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(5);
    m_expressionBar->enterPointEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋0.1");
    DSettings::deleteInstance();
}

//TEST_F(Ut_SciexpressionBar, enterBackspaceEvent)
//{
//    SciExpressionBar *m_expressionBar = new SciExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
//    m_expressionBar->allElection();
//    m_expressionBar->enterBackspaceEvent();
//    m_expressionBar->findChild<InputEdit *>()->setText("1.＋1");
//    m_expressionBar->enterBackspaceEvent();
//    m_expressionBar->enterBackspaceEvent();
//    m_expressionBar->enterBackspaceEvent();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, enterClearEvent)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->m_isAllClear = false;
//    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
//    m_expressionBar->enterClearEvent();
//    m_expressionBar->m_isAllClear = true;
//    m_expressionBar->enterClearEvent();
//    m_expressionBar->m_isAllClear = false;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
//    m_expressionBar->enterClearEvent();
//    ASSERT_EQ(m_expressionBar->m_listModel->rowCount(QModelIndex()), 0);
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, enterEqualEvent)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋");
//    historicalLinkageIndex his;
//    his.linkageTerm = 0;
//    his.linkageValue = 3;
//    his.linkedItem = 1;
//    m_expressionBar->m_hisLink.append(his);
//    his.linkageTerm = 1;
//    his.linkageValue = 3;
//    his.linkedItem = 2;
//    m_expressionBar->m_hisLink.append(his);
//    m_expressionBar->m_hisRevision = -1;
//    m_expressionBar->m_isLinked = true;
//    m_expressionBar->enterEqualEvent();
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
//    m_expressionBar->enterEqualEvent();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "2");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, enterPercentEventCommon)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->enterPercentEventCommon();
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
//    m_expressionBar->enterPercentEventCommon();
//    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
//    m_expressionBar->enterPercentEventCommon();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "0.01＋0.01");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, enterBracketsEvent)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
//    m_expressionBar->findChild<InputEdit *>()->setText("3");
//    m_expressionBar->m_isUndo = true;
//    m_expressionBar->enterBracketsEvent();
//    m_expressionBar->m_hisLink.last().linkedItem = -1;
//    m_expressionBar->enterBracketsEvent();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3(())");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, enterLeftBracketsEvent)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
//    m_expressionBar->findChild<InputEdit *>()->setText("3");
//    m_expressionBar->m_isUndo = true;
//    m_expressionBar->enterLeftBracketsEvent();
//    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
//    m_expressionBar->m_hisLink.last().linkedItem = -1;
//    m_expressionBar->enterLeftBracketsEvent();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "(3(");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, enterRightBracketsEvent)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
//    m_expressionBar->findChild<InputEdit *>()->setText("3");
//    m_expressionBar->m_isUndo = true;
//    m_expressionBar->enterRightBracketsEvent();
//    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
//    m_expressionBar->m_hisLink.last().linkedItem = -1;
//    m_expressionBar->enterRightBracketsEvent();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), ")3)");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, moveLeft)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
//    m_expressionBar->moveLeft();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 2);
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, moveRight)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
//    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
//    m_expressionBar->moveRight();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 1);
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, copyResultToClipboard)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
//    m_expressionBar->allElection();
//    m_expressionBar->copyResultToClipboard();
//    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
//    m_expressionBar->findChild<InputEdit *>()->setText("3");
//    m_expressionBar->m_isUndo = true;
//    m_expressionBar->copyClipboard2Result();
//    m_expressionBar->m_hisLink.last().linkedItem = -1;
//    m_expressionBar->copyClipboard2Result();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "31＋21＋2");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, allElection)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
//    m_expressionBar->allElection();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->getSelection().selected, "1＋2");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, shear)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
//    m_expressionBar->allElection();
//    m_expressionBar->shear();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, clearLinkageCache)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    historicalLinkageIndex his;
//    his.linkageTerm = 0;
//    his.linkageValue = 3;
//    his.linkedItem = 1;
//    m_expressionBar->m_hisLink.append(his);
//    m_expressionBar->clearLinkageCache("1＋2＝3", true);
//    historicalLinkageIndex his1;
//    his1.linkageTerm = 0;
//    his1.linkageValue = 3;
//    his1.linkedItem = 1;
//    m_expressionBar->m_hisLink.append(his1);
//    m_expressionBar->m_hisLink.last().linkedItem = -1;
//    m_expressionBar->clearLinkageCache("1＋2＝3", false);
////    m_expressionBar->shear();
//    ASSERT_EQ(m_expressionBar->m_hisLink.count(), 0);
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, setResultFalse)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->m_isResult = true;
//    m_expressionBar->setResultFalse();
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, replaceSelection)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
//    m_expressionBar->allElection();
//    m_expressionBar->replaceSelection("1＋2");
//    ASSERT_FALSE(m_expressionBar->m_isResult);
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, revisionResults)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
//    m_expressionBar->revisionResults(m_expressionBar->m_listModel->index(0, 0));
//    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋2");
//    DSettings::deleteInstance();
//}

//TEST_F(Ut_SciexpressionBar, cancelLink)
//{
//    ExpressionBar *m_expressionBar = new ExpressionBar;
//    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
//    historicalLinkageIndex his;
//    his.linkageTerm = 0;
//    his.linkageValue = 3;
//    his.linkedItem = 1;
//    m_expressionBar->m_hisLink.append(his);
//    m_expressionBar->m_hisRevision = 1;
//    ASSERT_TRUE(m_expressionBar->cancelLink(1));
//    DSettings::deleteInstance();
//}
