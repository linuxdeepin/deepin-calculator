#include "ut_sciexpressionbar.h"

#include "../../src/widgets/sciexpressionbar.h"

Ut_SciexpressionBar::Ut_SciexpressionBar()
{

}

TEST_F(Ut_SciexpressionBar, mouseMoveEvent)
{
    SciExpressionBar *m_sciexpressionBar = new SciExpressionBar;
    m_sciexpressionBar->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_sciexpressionBar->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    //无ASSERT
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, getanswer)
{
    SciExpressionBar *m_sciexpressionBar = new SciExpressionBar;
    m_sciexpressionBar->m_pair.first = false;
    ASSERT_TRUE(m_sciexpressionBar->getanswer().isZero());
    //无ASSERT
    DSettingsAlt::deleteInstance();
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
    DSettingsAlt::deleteInstance();
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
    DSettingsAlt::deleteInstance();
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
    DSettingsAlt::deleteInstance();
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
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterBackspaceEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->allElection();
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1.＋1");
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->findChild<InputEdit *>()->clear();
    m_expressionBar->enterFunctionEvent("sin");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterBackspaceEvent();
    m_expressionBar->enterFunctionEvent("sin");
    SSelection select;
    select.curpos = 1;
    select.oldText = "sin()";
    select.selected = "i";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select);
    m_expressionBar->enterBackspaceEvent();
    qDebug() << m_expressionBar->findChild<InputEdit *>()->text();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "()()");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterClearEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->m_isAllClear = false;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->enterClearEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterEqualEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("0log1");
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1");
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1÷3000");
    m_expressionBar->enterEqualEvent();
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋");
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->m_FEisdown = true;
    m_expressionBar->enterEqualEvent();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->m_FEisdown = false;
    m_expressionBar->enterEqualEvent();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterLeftBracketsEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterBracketEvent(1);
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterBracketEvent(1);
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterBracketEvent(1);
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1(111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterRightBracketsEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("3");
    m_expressionBar->m_isUndo = true;
    m_expressionBar->enterBracketEvent(2);
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterBracketEvent(2);
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterBracketEvent(2);
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1)111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterFEEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterFEEvent(true);
    ASSERT_FALSE(m_expressionBar->m_FEisdown);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterModEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterOperatorEvent("mod");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterOperatorEvent("mod");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(6);
    m_expressionBar->enterOperatorEvent("mod");
    m_expressionBar->findChild<InputEdit *>()->setText("1^2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterOperatorEvent("mod");
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterOperatorEvent("mod");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1mod111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterx2Event)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterOperatorEvent("^2");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterOperatorEvent("^2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(6);
    m_expressionBar->enterOperatorEvent("^2");
    m_expressionBar->findChild<InputEdit *>()->setText("1^2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterOperatorEvent("^2");
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterOperatorEvent("^2");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1^2×111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterDerivativeEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->allElection();
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterSpecialFuncEvent("1/(");
    SSelection select;
    select.curpos = 0;
    select.oldText = "1＋1";
    select.selected = "1";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select);
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("1E-2");
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("－");
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("()");
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("((1))");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(4);
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("sin(2)");
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("1/(2)");
    m_expressionBar->enterSpecialFuncEvent("1/(");
    m_expressionBar->findChild<InputEdit *>()->setText("2(2)");
    m_expressionBar->enterSpecialFuncEvent("1/(");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "2×1/((2))");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterCosEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterFunctionEvent("cos");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1cos()111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enter10xEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1");
    m_expressionBar->enterConstantEvent("10^");
    m_expressionBar->findChild<InputEdit *>()->setText("11");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->enterConstantEvent("10^");
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->enterConstantEvent("10^");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋");
    m_expressionBar->enterConstantEvent("10^");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋10^");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, enterModulusEvent)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("E");
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->allElection();
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->enterSpecialFuncEvent("abs(");
    SSelection select;
    select.curpos = 0;
    select.oldText = "1＋1";
    select.selected = "1";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select);
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("1E-2");
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("－");
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("()");
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("((1))");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(4);
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("sin(2)");
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("1/(2)");
    m_expressionBar->enterSpecialFuncEvent("abs(");
    m_expressionBar->findChild<InputEdit *>()->setText("2(2)");
    m_expressionBar->enterSpecialFuncEvent("abs(");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "2abs((2))");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, copyResultToClipboard)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->copyResultToClipboard();
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->copyResultToClipboard();
    m_expressionBar->copyClipboard2Result();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, copyClipboard2Result)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("huhuisinsjoi");
    m_expressionBar->allElection();
    m_expressionBar->copyResultToClipboard();
    m_expressionBar->copyClipboard2Result();
    m_expressionBar->findChild<InputEdit *>()->setText("1.2＋3.4");
    SSelection select;
    select.curpos = 3;
    select.oldText = "1.2＋3.4";
    select.selected = "＋";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select);
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(3);
    m_expressionBar->copyClipboard2Result();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1.2sin34");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, shear)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->shear();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, revisionResults)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->revisionResults(m_expressionBar->m_listModel->index(0, 0));
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1＋2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, hisRevisionResults)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->hisRevisionResults(m_expressionBar->m_listModel->index(0, 0), Quantity(3));
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "3");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, judgeinput)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("sin");
    SSelection select;
    select.curpos = 1;
    select.oldText = "sin";
    select.selected = "i";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select);
    m_expressionBar->judgeinput();
    m_expressionBar->findChild<InputEdit *>()->setText("1sin");
    SSelection select0;
    select0.curpos = 0;
    select0.oldText = "1sin";
    select0.selected = "1";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select0);
    m_expressionBar->judgeinput();
    m_expressionBar->findChild<InputEdit *>()->setText("1sin");
    SSelection select1;
    select1.curpos = 0;
    select1.oldText = "1sin";
    select1.selected = "1s";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select1);
    ASSERT_FALSE(m_expressionBar->judgeinput());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, symbolComplement)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->symbolComplement("1(");
    m_expressionBar->symbolComplement(")1");
    ASSERT_EQ(m_expressionBar->symbolComplement("e"), "(e)");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, pointFaultTolerance)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->pointFaultTolerance(").");
    m_expressionBar->pointFaultTolerance(".1");
    ASSERT_EQ(m_expressionBar->pointFaultTolerance("1+.2"), "1＋0.2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, expressionCheck)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText(".1");
    m_expressionBar->expressionCheck();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "0.1");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, Undo)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->Undo();
    m_expressionBar->findChild<InputEdit *>()->setText("1");
    m_expressionBar->m_isResult = true;
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->m_undo.append("1");
    m_expressionBar->Undo();
    ASSERT_TRUE(m_expressionBar->m_isAllClear);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, Redo)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->Redo();
    m_expressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_expressionBar->m_redo.append("1");
    m_expressionBar->m_redo.append("");
    m_expressionBar->Redo();
    ASSERT_TRUE(m_expressionBar->m_isAllClear);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, initTheme)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->initTheme(0);
    if (DGuiApplicationHelper::instance()->themeType() == 1)
        ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text), "#303030");
    else
        ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text), "#B4B4B4");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, setResultFalse)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->setResultFalse();
    ASSERT_FALSE(m_expressionBar->m_isResult);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, replaceSelection)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1111");
    SSelection select;
    select.curpos = 2;
    select.oldText = "1,111";
    select.selected = "1";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select);
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_expressionBar->replaceSelection("1,111");
    m_expressionBar->findChild<InputEdit *>()->setText("1.111");
    SSelection select1;
    select1.curpos = 1;
    select1.oldText = "1.111";
    select1.selected = ".";
    m_expressionBar->findChild<InputEdit *>()->setSelection(select1);
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_expressionBar->replaceSelection("1.111");
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "1,111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, moveLeft)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->moveLeft();
    m_expressionBar->findChild<InputEdit *>()->setText("sin2");
    m_expressionBar->moveLeft();
    m_expressionBar->moveLeft();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 0);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, moveRight)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->moveRight();
    m_expressionBar->findChild<InputEdit *>()->setText("sin2");
    m_expressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_expressionBar->moveRight();
    m_expressionBar->moveRight();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->cursorPosition(), 4);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SciexpressionBar, deleteText)
{
    SciExpressionBar *m_expressionBar = new SciExpressionBar;
    m_expressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_expressionBar->allElection();
    m_expressionBar->deleteText();
    ASSERT_EQ(m_expressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}
