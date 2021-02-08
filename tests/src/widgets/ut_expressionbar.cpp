#include "ut_expressionbar.h"

#include "../../src/widgets/expressionbar.h"

Ut_ExpressionBar::Ut_ExpressionBar()
{

}

TEST_F(Ut_ExpressionBar, mouseMoveEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_expressionBar->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    //无ASSERT
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterNumberEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_inputNumber = true;
    m_expressionBar->m_hisRevision = -1;
    m_expressionBar->enterNumberEvent("1");
    m_expressionBar->m_isResult = true;
    m_expressionBar->enterNumberEvent("1");
    m_expressionBar->m_isContinue = false;
    m_expressionBar->enterNumberEvent("1");
    m_expressionBar->enterNumberEvent("1");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "11");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterSymbolEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterSymbolEvent("-");
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterSymbolEvent("＋");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterSymbolEvent("-");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterSymbolEvent("-");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "－3＋");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterPointEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->enterPointEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1");
    m_expressionBar->enterPointEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterPointEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋0.1");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterBackspaceEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->allElection();
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1.＋1");
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->enterBackspaceEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1");
    DSettingsAlt::deleteInstance();
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
    ASSERT_EQ(m_expressionBar->m_listModel->rowCount(QModelIndex()), 0);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterEqualEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋");
    historicalLinkageIndex his;
    his.linkageTerm = 0;
    his.linkageValue = 3;
    his.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his);
    his.linkageTerm = 1;
    his.linkageValue = 3;
    his.linkedItem = 2;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->m_hisRevision = -1;
    m_expressionBar->m_isLinked = true;
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterEqualEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterPercentEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->enterPercentEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterPercentEvent();
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterPercentEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1%＋1%");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterBracketsEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterBracketsEvent();
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterBracketsEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3(())");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterLeftBracketsEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterLeftBracketsEvent();
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterLeftBracketsEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "(3(");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, enterRightBracketsEvent)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterRightBracketsEvent();
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->enterRightBracketsEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), ")3)");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, moveLeft)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->moveLeft();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 2);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, moveRight)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->moveRight();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 1);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, copyResultToClipboard)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->copyResultToClipboard();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->copyResultToClipboard();
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->copyClipboard2Result();
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->copyClipboard2Result();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "31＋21＋2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, allElection)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->getSelection().selected, "1＋2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, shear)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->shear();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, clearLinkageCache)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    historicalLinkageIndex his;
    his.linkageTerm = 0;
    his.linkageValue = 3;
    his.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->clearLinkageCache("1＋2＝3", true);
    historicalLinkageIndex his1;
    his1.linkageTerm = 0;
    his1.linkageValue = 3;
    his1.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his1);
    m_expressionBar->m_hisLink.last().linkedItem = -1;
    m_expressionBar->clearLinkageCache("1＋2＝3", false);
//    m_expressionBar->shear();
    ASSERT_EQ(m_expressionBar->m_hisLink.count(), 0);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, setResultFalse)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_isResult = true;
    m_expressionBar->setResultFalse();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, replaceSelection)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->replaceSelection("1＋2");
    ASSERT_FALSE(m_expressionBar->m_isResult);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, revisionResults)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->revisionResults(m_expressionBar->m_listModel->index(0, 0));
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ExpressionBar, cancelLink)
{
    ExpressionBar *m_expressionBar = new ExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    historicalLinkageIndex his;
    his.linkageTerm = 0;
    his.linkageValue = 3;
    his.linkedItem = 1;
    m_expressionBar->m_hisLink.append(his);
    m_expressionBar->m_hisRevision = 1;
    ASSERT_TRUE(m_expressionBar->cancelLink(1));
    DSettingsAlt::deleteInstance();
}
