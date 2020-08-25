#include "ut_scientificmodule.h"
#define private public
#define protected public
#include "src/widgets/scientificmodule.h"
#include "src/views/memoryitemwidget.h"
#undef private
#undef protected
#include "src/memorypublic.h"

Ut_ScientificModule::Ut_ScientificModule()
{

}

TEST_F(Ut_ScientificModule, connect)
{
    scientificModule *m_scientificModule = new scientificModule;
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_1), Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_Plus), Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_2), Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_Equals), Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_Clear), Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_1), Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_MS), Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<MemoryItemWidget *>()->m_btnplus, Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<MemoryItemWidget *>()->m_btnminus, Qt::LeftButton);
    QTest::mouseClick(m_scientificModule->findChild<MemoryItemWidget *>(), Qt::LeftButton);
    ASSERT_EQ(m_scientificModule->findChild<InputEdit *>()->text(), "1");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, setKeyPress)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->setKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    ASSERT_EQ(m_scientificModule->findChild<InputEdit *>()->text(), "1");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, showOrHideHistory)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->showOrHideHistory(true);
    m_scientificModule->showOrHideHistory(false);
    ASSERT_FALSE(m_scientificModule->m_scihiswidget->isHidden());
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, checkLineEmpty)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->checkLineEmpty();
    m_scientificModule->findChild<InputEdit *>()->setText("1");
    m_scientificModule->checkLineEmpty();
    ASSERT_TRUE(m_scientificModule->m_memCalbtn);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, mAvailableEvent)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->mAvailableEvent();
    ASSERT_TRUE(m_scientificModule->m_memRCbtn);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, mUnAvailableEvent)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->mUnAvailableEvent();
    ASSERT_FALSE(m_scientificModule->m_memRCbtn);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, initTheme)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->initTheme(1);
    ASSERT_EQ(m_scientificModule->findChild<InputEdit *>()->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text), "#303030");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, handleEditKeyPress)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_2, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_4, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_5, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_6, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_7, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_8, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_9, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Underscore, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Slash, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Equal, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenLeft, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenRight, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Percent, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_AsciiCircum, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Z, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::NoModifier));
    m_scientificModule->m_memRCbtn = true;
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::NoModifier));
    m_scientificModule->m_memCalbtn = true;
    m_scientificModule->findChild<InputEdit *>()->setText("1");
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_NumberSign, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_At, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Bar, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Exclam, Qt::NoModifier));
    //函数中输入
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_sin), Qt::LeftButton);
    m_scientificModule->findChild<InputEdit *>()->setCursorPosition(1);
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_2, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_4, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_5, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_6, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_7, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_8, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_9, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Underscore, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Slash, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenLeft, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenRight, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenRight, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Percent, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_AsciiCircum, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_NumberSign, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::ControlModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_At, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Bar, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::ShiftModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Exclam, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_W, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_scientificModule->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Equal, Qt::NoModifier));
    ASSERT_EQ(m_scientificModule->findChild<InputEdit *>()->text(), "2");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, handleKeypadButtonPress)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_0);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_2);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_3);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_4);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_5);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_6);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_7);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_8);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_9);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Plus);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Min);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Mult);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Div);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Percent);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Equals);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Clear);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Backspace);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Point);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_left);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_right);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Clear);
    m_scientificModule->findChild<InputEdit *>()->setText("1");
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_MS);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_MC);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Mplus);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Mmin);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_MR);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_deg);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_sin);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_FE);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_page);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_PI);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_e);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Mod);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_x2);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Derivative);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Factorials);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_exp);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_cos);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_x3);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_tan);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_xy);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_cot);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_10x);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Modulus);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_log);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Rand);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_ln);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arcsin);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_sqrt2);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arccos);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_sqrt3);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arctan);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_ysqrtx);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arccot);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_2x);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_logyx);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_ex);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_opposite);
    m_scientificModule->handleKeypadButtonPress(84);
    //函数中输入
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Clear);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_sin), Qt::LeftButton);
    m_scientificModule->findChild<InputEdit *>()->setCursorPosition(1);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_0);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_2);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_3);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_4);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_5);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_6);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_7);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_8);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_9);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Plus);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Min);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Mult);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Div);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Percent);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Point);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_left);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_right);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_sin);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_PI);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_e);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Mod);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_x2);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Derivative);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Factorials);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_exp);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_cos);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_x3);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_tan);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_xy);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_cot);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_10x);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Modulus);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_log);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_ln);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arcsin);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_sqrt2);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arccos);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_sqrt3);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arctan);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_ysqrtx);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_arccot);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_2x);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_logyx);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_ex);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_opposite);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Clear);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Plus);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPress(ScientificKeyPad::Key_Equals);
    ASSERT_EQ(m_scientificModule->findChild<InputEdit *>()->text(), "2");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificModule, handleKeypadButtonPressByspace)
{
    scientificModule *m_scientificModule = new scientificModule;
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_0);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_2);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_3);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_4);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_5);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_6);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_7);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_8);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_9);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Plus);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Min);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Mult);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Div);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Percent);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Equals);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Clear);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Backspace);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Point);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_left);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_right);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Clear);
    m_scientificModule->findChild<InputEdit *>()->setText("1");
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_MS);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_MC);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Mplus);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Mmin);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_MR);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_deg);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_deg);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_deg);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_sin);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_FE);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_page);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_page);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_PI);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_e);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Mod);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_x2);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Derivative);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Factorials);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_exp);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_cos);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_x3);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_tan);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_xy);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_cot);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_10x);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Modulus);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_log);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Rand);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_ln);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arcsin);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_sqrt2);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arccos);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_sqrt3);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arctan);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_ysqrtx);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arccot);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_2x);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_logyx);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_ex);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_opposite);
    //函数中输入
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Clear);
    QTest::mouseClick(m_scientificModule->findChild<ScientificKeyPad *>()->button(ScientificKeyPad::Key_sin), Qt::LeftButton);
    m_scientificModule->findChild<InputEdit *>()->setCursorPosition(1);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_0);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_2);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_3);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_4);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_5);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_6);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_7);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_8);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_9);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Plus);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Min);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Mult);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Div);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Percent);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Point);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_left);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_right);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_sin);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_PI);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_e);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Mod);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_x2);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Derivative);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Factorials);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_exp);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_cos);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_x3);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_tan);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_xy);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_cot);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_10x);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Modulus);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_log);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_ln);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arcsin);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_sqrt2);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arccos);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_sqrt3);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arctan);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_ysqrtx);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_arccot);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_2x);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_logyx);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_ex);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_opposite);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Clear);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Plus);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_1);
    m_scientificModule->handleKeypadButtonPressByspace(ScientificKeyPad::Key_Equals);
    ASSERT_EQ(m_scientificModule->findChild<InputEdit *>()->text(), "2");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

