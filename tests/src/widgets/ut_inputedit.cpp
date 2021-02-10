#include "ut_inputedit.h"

#include "../../src/widgets/inputedit.h"
#include <QAction>
#include "../../3rdparty/math/cmath.h"

Ut_InputEdit::Ut_InputEdit()
{

}

TEST_F(Ut_InputEdit, expressionText)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("3.333333333333333E－4");
    m_inputEdit->m_evaluator = Evaluator::instance();
    m_inputEdit->m_evaluator->setExpression("1/3000");
    m_inputEdit->m_ans = m_inputEdit->m_evaluator->evalUpdateAns();
    m_inputEdit->m_ansVaild = true;
    m_inputEdit->m_strans = QString("3.333333333333333E－4");
    m_inputEdit->m_ansStartPos = 0;
    m_inputEdit->m_ansLength = 20;
    ASSERT_EQ(m_inputEdit->expressionText(), "lastans");
}

TEST_F(Ut_InputEdit, setAnswer)
{
    InputEdit *m_inputEdit = new InputEdit;
    Evaluator *m_evaluator = Evaluator::instance();
    m_evaluator->setExpression("1/3000");
    m_inputEdit->setAnswer("3.333333333333333E－4", m_evaluator->evalUpdateAns());
    ASSERT_EQ(m_inputEdit->m_ansLength, 20);
}

TEST_F(Ut_InputEdit, keyPressEvent)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setFocus();
    m_inputEdit->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::AltModifier));
    //无ASSERT
}

TEST_F(Ut_InputEdit, mouseDoubleClickEvent)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("12345");
    m_inputEdit->mouseDoubleClickEvent(new QMouseEvent(QEvent::MouseButtonDblClick, m_inputEdit->rect().center(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
    ASSERT_EQ(m_inputEdit->m_selected.selected, "12,345");
}

TEST_F(Ut_InputEdit, mousePressEvent)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("12345");
    m_inputEdit->mouseDoubleClickEvent(new QMouseEvent(QEvent::MouseButtonDblClick, m_inputEdit->rect().center(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
    m_inputEdit->mousePressEvent(new QMouseEvent(QEvent::MouseButtonPress, m_inputEdit->rect().center(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
    ASSERT_EQ(m_inputEdit->m_selected.selected, "");
}

TEST_F(Ut_InputEdit, handleTextChanged)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->handleTextChanged("1=");
    ASSERT_EQ(m_inputEdit->text(), "1");
}

TEST_F(Ut_InputEdit, pointFaultTolerance)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->pointFaultTolerance("1");
    m_inputEdit->pointFaultTolerance(".1");
    m_inputEdit->pointFaultTolerance("%.1");
    ASSERT_EQ(m_inputEdit->pointFaultTolerance("0.1.1"), "0.11");
}

TEST_F(Ut_InputEdit, symbolFaultTolerance)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->symbolFaultTolerance("－1");
    m_inputEdit->symbolFaultTolerance("E(－");
    ASSERT_EQ(m_inputEdit->symbolFaultTolerance("1E－.1－1"), "1E－1－1");
}

TEST_F(Ut_InputEdit, isSymbol)
{
    InputEdit *m_inputEdit = new InputEdit;
    ASSERT_TRUE(m_inputEdit->isSymbol("－"));
}

TEST_F(Ut_InputEdit, handleCursorPositionChanged)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->m_ansStartPos = 0;
    m_inputEdit->m_ansLength = 3;
    m_inputEdit->m_selected.curpos = 1;
    m_inputEdit->m_selected.selected = "2";
    m_inputEdit->handleCursorPositionChanged(0, 0);
    ASSERT_TRUE(m_inputEdit->m_currentInAns);
}

TEST_F(Ut_InputEdit, BracketCompletion)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("(()");
    m_inputEdit->BracketCompletion(new QKeyEvent(QEvent::None, Qt::Key_1, Qt::NoModifier));
    m_inputEdit->setText("())");
    m_inputEdit->BracketCompletion(new QKeyEvent(QEvent::None, Qt::Key_1, Qt::NoModifier));
    m_inputEdit->setText("()");
    m_inputEdit->BracketCompletion(new QKeyEvent(QEvent::None, Qt::Key_1, Qt::NoModifier));
    ASSERT_EQ(m_inputEdit->text(), "()()");
}

TEST_F(Ut_InputEdit, multipleArithmetic)
{
    InputEdit *m_inputEdit = new InputEdit;
    QString s = "\n)\n";
    m_inputEdit->multipleArithmetic(s);
    ASSERT_EQ(s, "\n)×");
}

TEST_F(Ut_InputEdit, getMemoryAnswer)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("1log0");
    m_inputEdit->getMemoryAnswer();
    ASSERT_FALSE(m_inputEdit->getMemoryAnswer().first);
}

TEST_F(Ut_InputEdit, hisexpression)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->m_ansLength = 1;
    m_inputEdit->hisexpression();
    ASSERT_EQ(m_inputEdit->m_ansLength, 0);
}

TEST_F(Ut_InputEdit, symbolComplement)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->symbolComplement("1(2)");
    ASSERT_EQ(m_inputEdit->symbolComplement("(1)2"), "(1)×2");
}

TEST_F(Ut_InputEdit, focusInEvent)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->focusInEvent(new QFocusEvent(QEvent::Type::FocusIn, Qt::TabFocusReason));
    //无ASSERT
}

TEST_F(Ut_InputEdit, clear)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->clear();
    ASSERT_EQ(m_inputEdit->m_ansLength, 0);
}

TEST_F(Ut_InputEdit, setUndoAction)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setUndoAction(false);
    ASSERT_FALSE(m_inputEdit->m_undo->isEnabled());
}

TEST_F(Ut_InputEdit, setRedoAction)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setRedoAction(true);
    ASSERT_TRUE(m_inputEdit->m_redo->isEnabled());
}

TEST_F(Ut_InputEdit, autoZoomFontSize)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("0");
    m_inputEdit->autoZoomFontSize();
    ASSERT_EQ(m_inputEdit->font().pixelSize(), 30);
}

TEST_F(Ut_InputEdit, themetypechanged)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->themetypechanged(2);
    m_inputEdit->themetypechanged(1);
    ASSERT_EQ(m_inputEdit->palette().text(), QBrush("#303030"));
}

TEST_F(Ut_InputEdit, valueChangeFromProSyskeypad)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("");
    m_inputEdit->setCursorPosition(0);
    m_inputEdit->valueChangeFromProSyskeypad("11100011");
    ASSERT_EQ(m_inputEdit->cursorPosition(), 3);
}

TEST_F(Ut_InputEdit, radixChanged)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("120+323+456");
    m_inputEdit->radixChanged(10, 16);
    ASSERT_EQ(m_inputEdit->text(), "78＋143＋1C8");
}

TEST_F(Ut_InputEdit, formatAns)
{
    InputEdit *m_inputEdit = new InputEdit;
    QString ans = m_inputEdit->formatAns("11100101");
    ASSERT_EQ(ans, "229");
}

TEST_F(Ut_InputEdit, getCurrentAns)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("0x1C8");
    QPair<bool, Quantity> pair = m_inputEdit->getCurrentAns();
    int ans = pair.second.numericValue().toInt();
    ASSERT_EQ(ans, 456);
}

TEST_F(Ut_InputEdit, CurrentCursorPositionNumber)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->setText("6666+2333");
    m_inputEdit->setCursorPosition(9);
    QString str = m_inputEdit->CurrentCursorPositionNumber(m_inputEdit->cursorPosition());
    ASSERT_EQ(str, "2333");
}

TEST_F(Ut_InputEdit, isNumber)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->isNumber('a');
    m_inputEdit->isNumber('A');
    ASSERT_TRUE(m_inputEdit->isNumber('A'));
}

TEST_F(Ut_InputEdit, formatBinaryNumber)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->formatBinaryNumber("0000000000");
    QString str = m_inputEdit->formatBinaryNumber("00000000111001");
    ASSERT_EQ(str, "57");
}

TEST_F(Ut_InputEdit, formatExpression)
{
    InputEdit *m_inputEdit = new InputEdit;
    m_inputEdit->formatExpression(16, "4435");
    m_inputEdit->formatExpression(10, "4435");
    m_inputEdit->formatExpression(2, "4435");
    QString str = m_inputEdit->formatExpression(8, "4435");
    ASSERT_EQ(str, "0o4435");
}
