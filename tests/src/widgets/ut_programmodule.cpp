#include <QPalette>
#include "ut_programmodule.h"

#include "../../src/widgets/programmodule.h"
#include "../../3rdparty/core/settings.h"
#include <QPair>

Ut_ProgramModule::Ut_ProgramModule()
{

}

TEST_F(Ut_ProgramModule, Signals)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->m_proExpressionBar->getInputEdit()->cursorPositionNumberChanged("1");
    QPair<bool, Quantity> pair = {false, Quantity(1)};
    m_programmodule->m_proExpressionBar->getInputEdit()->prolistAns(pair);
    QPair<bool, Quantity> pair1 = {true, Quantity(1)};
    m_programmodule->m_proExpressionBar->getInputEdit()->prolistAns(pair1);
    m_programmodule->m_proExpressionBar->getInputEdit()->emptyExpression(true);
    m_programmodule->m_proExpressionBar->getInputEdit()->emptyExpression(false);

    m_programmodule->m_memorylistwidget->widgetplus(0);
    m_programmodule->m_memoryPublic->generateData(Quantity(1));
    m_programmodule->m_memoryPublic->memoryclean();
    m_programmodule->m_stackWidget->setCurrentIndex(2);
    QPair<QString, Quantity> pair2 = {"1", Quantity(1)};
    m_programmodule->m_memorylistwidget->itemclick(pair2);
    m_programmodule->m_memorylistwidget->hideWidget();
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, Signals1)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->m_checkBtnKeypad->buttonPressed(36);
    m_programmodule->m_checkBtnKeypad->buttonPressedbySpace(36);
    m_programmodule->m_programmerKeypad->buttonPressed(26);
    m_programmodule->m_programmerKeypad->buttonPressedbySpace(26);
    m_programmodule->m_proSystemKeypad->bitbuttonclicked();
    ASSERT_EQ(m_programmodule->findChild<InputEdit *>()->text(), "11");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, setKeyPress)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->setKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    ASSERT_EQ(m_programmodule->findChild<InputEdit *>()->text(), "1");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, mAvailableEvent)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->mAvailableEvent();
    ASSERT_TRUE(m_programmodule->m_avail);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, mUnAvailableEvent)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->mUnAvailableEvent();
    ASSERT_FALSE(m_programmodule->m_avail);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, showListWidget)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->showListWidget();
    ASSERT_EQ(m_programmodule->m_stackWidget->currentIndex(), 2);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, closeListWidget)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->closeListWidget();
    ASSERT_EQ(m_programmodule->m_stackWidget->currentIndex(), 0);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, handleEditKeyPress)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_2, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_4, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_5, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_6, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_7, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_8, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_9, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Underscore, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Slash, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Equal, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenLeft, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenRight, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Percent, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_AsciiCircum, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Z, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::NoModifier));
    m_programmodule->m_memCalbtn = true;
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::NoModifier));
    m_programmodule->m_memCalbtn = true;
    m_programmodule->findChild<InputEdit *>()->setText("1");
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_NumberSign, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_At, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Bar, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Exclam, Qt::NoModifier));
    //函数中输入
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    QTest::mouseClick(m_programmodule->findChild<ProgrammerKeypad *>()->button(ProgrammerKeypad::Key_AND), Qt::LeftButton);
    m_programmodule->findChild<InputEdit *>()->setCursorPosition(1);
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_2, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_4, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_5, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_6, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_7, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_8, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_9, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Underscore, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Slash, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenLeft, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenRight, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenRight, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Percent, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_AsciiCircum, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_NumberSign, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::ControlModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_At, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Bar, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::ShiftModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Exclam, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_W, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_programmodule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Equal, Qt::NoModifier));
    ASSERT_EQ(m_programmodule->findChild<InputEdit *>()->text(), "2");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, handleKeypadButtonPress)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->handleKeypadButtonPress(ProCheckBtnKeypad::Key_BinaryKeypad);
    m_programmodule->handleKeypadButtonPress(ProCheckBtnKeypad::Key_GeneralKeypad);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_0);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_1);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_2);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_3);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_4);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_5);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_6);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_7);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_8);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_9);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Plus);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Min);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Mult);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Div);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_percent);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_moveL);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_moveR);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Clear);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Backspace);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_opposite);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_leftBracket);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_rightBracket);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Clear);
    m_programmodule->findChild<InputEdit *>()->setText("1");
    m_programmodule->handleKeypadButtonPress(ProCheckBtnKeypad::Key_BinaryKeypad);
    m_programmodule->handleKeypadButtonPress(ProCheckBtnKeypad::Key_GeneralKeypad);
    m_programmodule->handleKeypadButtonPress(ProCheckBtnKeypad::Key_System);
    m_programmodule->handleKeypadButtonPress(ProCheckBtnKeypad::Key_Option);
    m_programmodule->handleKeypadButtonPress(ProCheckBtnKeypad::Key_MS);
    //函数中输入
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Clear);
    QTest::mouseClick(m_programmodule->findChild<ProgrammerKeypad *>()->button(ProgrammerKeypad::Key_AND), Qt::LeftButton);
    m_programmodule->findChild<InputEdit *>()->setCursorPosition(1);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_0);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_1);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_2);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_3);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_4);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_5);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_6);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_7);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_8);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_9);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Plus);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Min);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Mult);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Div);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_percent);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_moveL);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_moveR);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Clear);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Backspace);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_opposite);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_leftBracket);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_rightBracket);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Clear);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_1);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_Plus);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_3);
    m_programmodule->handleKeypadButtonPress(ProgrammerKeypad::Key_equal);
    ASSERT_EQ(m_programmodule->findChild<InputEdit *>()->text(), "4");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, handleKeypadButtonPressByspace)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_0);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_1);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_2);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_3);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_4);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_5);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_6);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_7);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_8);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_9);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Plus);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Min);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Mult);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Div);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_percent);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_moveL);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_moveR);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Clear);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Backspace);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_opposite);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_leftBracket);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_rightBracket);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Clear);
    m_programmodule->findChild<InputEdit *>()->setText("1");
    m_programmodule->handleKeypadButtonPressByspace(ProCheckBtnKeypad::Key_BinaryKeypad);
    m_programmodule->handleKeypadButtonPressByspace(ProCheckBtnKeypad::Key_GeneralKeypad);
    m_programmodule->handleKeypadButtonPressByspace(ProCheckBtnKeypad::Key_System);
    m_programmodule->handleKeypadButtonPressByspace(ProCheckBtnKeypad::Key_Option);
    m_programmodule->handleKeypadButtonPressByspace(ProCheckBtnKeypad::Key_MS);
    //函数中输入
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Clear);
    QTest::mouseClick(m_programmodule->findChild<ProgrammerKeypad *>()->button(ProgrammerKeypad::Key_AND), Qt::LeftButton);
    m_programmodule->findChild<InputEdit *>()->setCursorPosition(1);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_0);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_1);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_2);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_3);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_4);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_5);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_6);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_7);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_8);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_9);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Plus);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Min);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Mult);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Div);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_percent);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_moveL);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_moveR);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Clear);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Backspace);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_opposite);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_leftBracket);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_rightBracket);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Clear);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_1);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_Plus);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_3);
    m_programmodule->handleKeypadButtonPressByspace(ProgrammerKeypad::Key_equal);
    ASSERT_EQ(m_programmodule->findChild<InputEdit *>()->text(), "4");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, shiftArrowListWidgetItemClicked)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->shiftArrowListWidgetItemClicked(2, false);
    m_programmodule->m_proListView->testAttribute(Qt::WA_TransparentForMouseEvents);
    ASSERT_EQ(m_programmodule->m_shiftArrowCurrentRow, 2);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, byteArrowListWidgetItemClicked)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->byteArrowListWidgetItemClicked(2, false);
    m_programmodule->m_proListView->testAttribute(Qt::WA_TransparentForMouseEvents);
    ASSERT_EQ(Settings::instance()->proBitLength, 16);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, handleClearStateChanged)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->handleClearStateChanged(true);
    ASSERT_EQ(m_programmodule->findChild<ProgrammerKeypad *>()->button(ProgrammerKeypad::Key_Clear)->text(), "AC");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, mousePressEvent)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                     m_programmodule->pos(), Qt::MouseButton::LeftButton,
                                                     Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_programmodule->m_insidewidget);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgramModule, setwidgetAttribute)
{
    ProgramModule *m_programmodule = new ProgramModule;
    m_programmodule->setwidgetAttribute(false);
    ASSERT_FALSE(m_programmodule->m_proExpressionBar->testAttribute(Qt::WA_TransparentForMouseEvents));
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}
