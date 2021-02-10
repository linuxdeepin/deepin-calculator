#include "ut_proexpressionbar.h"

#include "../../src/widgets/proexpressionbar.h"
#include "../../3rdparty/core/settings.h"
#include "../../src/mainwindow.h"
#include <QDir>
#include <QLocale>
#include <QSettings>
#include <QApplication>
#include <QFont>
#include <QtCore/QStandardPaths>

Ut_ProexpressionBar::Ut_ProexpressionBar()
{

}

TEST_F(Ut_ProexpressionBar, mouseMoveEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_proexpressionBar->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    //无ASSERT
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, isnumber)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    QChar a = 'A';
    m_proexpressionBar->isnumber(a);
    ASSERT_TRUE(m_proexpressionBar->m_inputEdit->isNumber(a));
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, judgeinput)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->m_inputEdit->setText("and");
    SSelection select;
    select.curpos = 1;
    select.oldText = "and";
    select.selected = "n";
    m_proexpressionBar->m_inputEdit->setSelection(select);
    bool b1 = m_proexpressionBar->judgeinput();
    m_proexpressionBar->m_inputEdit->setText("1and");
    SSelection select0;
    select0.curpos = 0;
    select0.oldText = "1and";
    select0.selected = "1";
    m_proexpressionBar->m_inputEdit->setSelection(select0);
    bool b2 = m_proexpressionBar->judgeinput();
    m_proexpressionBar->m_inputEdit->setText("1and");
    SSelection select1;
    select1.curpos = 0;
    select1.oldText = "1and";
    select1.selected = "1a";
    m_proexpressionBar->m_inputEdit->setSelection(select1);
    ASSERT_FALSE(b1 && b2 && m_proexpressionBar->judgeinput());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterNumberEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
//    Settings::instance()->programmerBase = 16;
    m_proexpressionBar->m_inputNumber = true;
    m_proexpressionBar->m_isResult = true;
    m_proexpressionBar->m_isContinue = false;
    m_proexpressionBar->enterNumberEvent("1");
    bool b1 = m_proexpressionBar->m_inputEdit->text().isEmpty();
    m_proexpressionBar->m_inputNumber = false;
    m_proexpressionBar->m_isResult = true;
    m_proexpressionBar->enterNumberEvent("1");
    m_proexpressionBar->enterNumberEvent("8");
    m_proexpressionBar->enterNumberEvent("2");
    m_proexpressionBar->enterNumberEvent("2");
    m_proexpressionBar->enterNumberEvent("2");
    bool b2 = m_proexpressionBar->m_inputEdit->text() == "18,222";
    ASSERT_TRUE(!b1 && b2);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterSymbolEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->enterSymbolEvent("＋");
    m_proexpressionBar->findChild<InputEdit *>()->clear();
    m_proexpressionBar->enterSymbolEvent("-");
    m_proexpressionBar->enterSymbolEvent("-");
    m_proexpressionBar->enterSymbolEvent("＋");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_proexpressionBar->enterSymbolEvent("-");
    m_proexpressionBar->findChild<InputEdit *>()->setText("1");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_proexpressionBar->enterSymbolEvent("-");
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_proexpressionBar->enterSymbolEvent("-");
    m_proexpressionBar->findChild<InputEdit *>()->setText("11");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_proexpressionBar->enterSymbolEvent("-");
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "1－1");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterBackspaceEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_proexpressionBar->allElection();
    m_proexpressionBar->enterBackspaceEvent();
    m_proexpressionBar->enterNotEvent();
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_proexpressionBar->enterBackspaceEvent();
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_proexpressionBar->enterOperatorEvent("and");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_proexpressionBar->enterBackspaceEvent();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "0(0)");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterClearEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->m_isAllClear = false;
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋1");
    m_proexpressionBar->enterClearEvent();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterEqualEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->enterNumberEvent("0");
    m_proexpressionBar->enterOperatorEvent("and");
    m_proexpressionBar->enterNumberEvent("1");
    m_proexpressionBar->enterEqualEvent();
    m_proexpressionBar->enterSymbolEvent("-");
    m_proexpressionBar->enterSymbolEvent("+");
    m_proexpressionBar->enterNumberEvent("5");
    m_proexpressionBar->enterEqualEvent();
    m_proexpressionBar->enterSymbolEvent("/");
    m_proexpressionBar->enterNumberEvent("3");
    m_proexpressionBar->enterOperatorEvent("or");
    m_proexpressionBar->enterNumberEvent("5");
    m_proexpressionBar->enterNumberEvent("6");
    m_proexpressionBar->enterEqualEvent();

    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "57");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterNotEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->enterNotEvent();
    m_proexpressionBar->enterClearEvent();
    m_proexpressionBar->enterNumberEvent("5");
    m_proexpressionBar->enterNotEvent();
    m_proexpressionBar->enterEqualEvent();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "－6");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterOperatorEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar;
    m_proexpressionBar->enterOperatorEvent("ror");
    m_proexpressionBar->enterClearEvent();
    m_proexpressionBar->enterNumberEvent("5");
    m_proexpressionBar->enterOperatorEvent("sal");
    m_proexpressionBar->enterOperatorEvent("rcl");
    m_proexpressionBar->enterOperatorEvent("ror");
    m_proexpressionBar->enterNumberEvent("4");
    m_proexpressionBar->enterEqualEvent();
    bool b1 = m_proexpressionBar->findChild<InputEdit *>()->text() == "5,764,607,523,034,234,880";
    m_proexpressionBar->enterOperatorEvent("rcl");
    m_proexpressionBar->enterNumberEvent("3");
    m_proexpressionBar->enterEqualEvent();
    bool b2 = m_proexpressionBar->findChild<InputEdit *>()->text() == "－9,223,372,036,854,775,807";
    ASSERT_TRUE(b1 && b2);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterOppositeEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->enterOppositeEvent();
    bool b1 = m_proexpressionBar->findChild<InputEdit *>()->text().isEmpty();
    m_proexpressionBar->enterNumberEvent("0");
    m_proexpressionBar->enterOppositeEvent();
    bool b2 = m_proexpressionBar->findChild<InputEdit *>()->text() == "0";
    m_proexpressionBar->enterClearEvent();
    m_proexpressionBar->enterSymbolEvent("-");
    m_proexpressionBar->enterNumberEvent("1");
    m_proexpressionBar->enterOppositeEvent();
    m_proexpressionBar->enterEqualEvent();
    bool b3 = m_proexpressionBar->findChild<InputEdit *>()->text() == "－1";
    ASSERT_TRUE(b1 && b2 && b3);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterLeftBracketsEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("3");
    m_proexpressionBar->m_isUndo = true;
    m_proexpressionBar->enterLeftBracketsEvent();
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_proexpressionBar->enterLeftBracketsEvent();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1111");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_proexpressionBar->enterLeftBracketsEvent();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "1(111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, enterRightBracketsEvent)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("3");
    m_proexpressionBar->m_isUndo = true;
    m_proexpressionBar->enterRightBracketsEvent();
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_proexpressionBar->enterRightBracketsEvent();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1111");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_proexpressionBar->enterRightBracketsEvent();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "1)111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, moveLeft)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_proexpressionBar->moveLeft();
    m_proexpressionBar->findChild<InputEdit *>()->setText("and2");
    m_proexpressionBar->moveLeft();
    m_proexpressionBar->moveLeft();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->cursorPosition(), 0);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, moveRight)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_proexpressionBar->moveRight();
    m_proexpressionBar->findChild<InputEdit *>()->setText("and2");
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(0);
    m_proexpressionBar->moveRight();
    m_proexpressionBar->moveRight();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->cursorPosition(), 4);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, initTheme)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->initTheme(0);
    if (DGuiApplicationHelper::instance()->themeType() == 1)
        ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text), "#303030");
    else
        ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text), "#B4B4B4");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, revisionResults)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_proexpressionBar->revisionResults(m_proexpressionBar->m_listModel->index(0, 0));
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "1＋2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, addUndo)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->m_inputEdit->setText("110,911");
    m_proexpressionBar->addUndo();
    ASSERT_TRUE(m_proexpressionBar->m_undo.at(0) == "110,911" && m_proexpressionBar->m_redo.isEmpty());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, Undo)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->Undo();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1");
    m_proexpressionBar->m_isResult = true;
    m_proexpressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_proexpressionBar->m_undo.append("1");
    m_proexpressionBar->Undo();
    ASSERT_TRUE(m_proexpressionBar->m_isAllClear);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, Redo)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->Redo();
    m_proexpressionBar->m_listModel->updataList(QString("1＋2") + "＝" + "3", -1);
    m_proexpressionBar->m_redo.append("1");
    m_proexpressionBar->m_redo.append("");
    m_proexpressionBar->Redo();
    ASSERT_TRUE(m_proexpressionBar->m_isAllClear);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, copyResultToClipboard)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->copyResultToClipboard();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_proexpressionBar->allElection();
    m_proexpressionBar->copyResultToClipboard();
    m_proexpressionBar->copyClipboard2Result();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "1＋2");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, copyClipboard2Result)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("huhuiandsjoi");
    m_proexpressionBar->allElection();
    m_proexpressionBar->copyResultToClipboard();
    m_proexpressionBar->copyClipboard2Result();
    m_proexpressionBar->findChild<InputEdit *>()->setText("112＋334");
    SSelection select;
    select.curpos = 3;
    select.oldText = "112＋334";
    select.selected = "＋";
    m_proexpressionBar->findChild<InputEdit *>()->setSelection(select);
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(3);
    m_proexpressionBar->copyClipboard2Result();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "112and334");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, allElection)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("123or321");
    m_proexpressionBar->allElection();
    ASSERT_EQ(m_proexpressionBar->m_inputEdit->getSelection().selected, "123or321") ;
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, shear)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_proexpressionBar->allElection();
    m_proexpressionBar->shear();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, deleteText)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1＋2");
    m_proexpressionBar->allElection();
    m_proexpressionBar->deleteText();
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, setResultFalse)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->setResultFalse();
    ASSERT_FALSE(m_proexpressionBar->m_isResult);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, replaceSelection)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("1111");
    SSelection select;
    select.curpos = 2;
    select.oldText = "1,111";
    select.selected = "1";
    m_proexpressionBar->findChild<InputEdit *>()->setSelection(select);
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(2);
    m_proexpressionBar->replaceSelection("1,111");
    m_proexpressionBar->findChild<InputEdit *>()->setText("1%111");
    SSelection select1;
    select1.curpos = 1;
    select1.oldText = "1.111";
    select1.selected = "%";
    m_proexpressionBar->findChild<InputEdit *>()->setSelection(select1);
    m_proexpressionBar->findChild<InputEdit *>()->setCursorPosition(1);
    m_proexpressionBar->replaceSelection("1%111");
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "1,111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, isNumberOutOfRange)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("9,223,372,036,854,775,807");
    ASSERT_TRUE(m_proexpressionBar->isNumberOutOfRange("1"));
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, selectedPartDelete)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->findChild<InputEdit *>()->setText("111and22");
    SSelection select1;
    select1.curpos = 2;
    select1.oldText = "111and22";
    select1.selected = "1an";
    m_proexpressionBar->m_inputEdit->setSelection(select1);
    QString sRegNum = "[a-z]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    m_proexpressionBar->selectedPartDelete(rx);
    ASSERT_EQ(m_proexpressionBar->findChild<InputEdit *>()->text(), "1,122");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, handleTextChanged)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    m_proexpressionBar->handleTextChanged();
    ASSERT_TRUE(!m_proexpressionBar->m_isAllClear && m_proexpressionBar->m_isContinue);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, isOperator)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    ASSERT_TRUE(m_proexpressionBar->isOperator(QString::fromUtf8("×")));
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProexpressionBar, symbolFaultTolerance)
{
    ProExpressionBar *m_proexpressionBar = new ProExpressionBar();
    QString str = m_proexpressionBar->symbolFaultTolerance("123＋－");
    ASSERT_EQ(str, "123－");
    DSettingsAlt::deleteInstance();
}

