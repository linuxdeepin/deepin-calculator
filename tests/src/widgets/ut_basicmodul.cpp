#include <QPalette>
#include "ut_basicmodul.h"

#include "../../src/widgets/basicmodule.h"

Ut_BasicModul::Ut_BasicModul()
{

}

TEST_F(Ut_BasicModul, setKeyPress)
{
    BasicModule *m_basicmodul = new BasicModule;
    m_basicmodul->setKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    ASSERT_EQ(m_basicmodul->findChild<InputEdit *>()->text(), "1");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, mAvailableEvent)
{
    BasicModule *m_basicmodul = new BasicModule;
    m_basicmodul->mAvailableEvent();
    ASSERT_TRUE(m_basicmodul->m_memRCbtn);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, mUnAvailableEvent)
{
    BasicModule *m_basicmodul = new BasicModule;
    m_basicmodul->mUnAvailableEvent();
    ASSERT_FALSE(m_basicmodul->m_memRCbtn);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, showListWidget)
{
    BasicModule *m_basicmodul = new BasicModule;
    m_basicmodul->showListWidget();
    ASSERT_EQ(m_basicmodul->m_keypadLayout->currentIndex(), 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, initTheme)
{
    BasicModule *m_basicmodul = new BasicModule;
    m_basicmodul->initTheme(1);
    ASSERT_EQ(m_basicmodul->findChild<InputEdit *>()->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text), "#303030");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, handleEditKeyPress)
{
    BasicModule *m_basicmodul = new BasicModule;
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_2, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_4, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_5, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_6, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_7, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_8, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_9, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Underscore, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Slash, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Equal, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenLeft, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenRight, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Percent, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Z, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::ControlModifier));
    m_basicmodul->m_memRCbtn = true;
    m_basicmodul->m_isallgray = false;
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::ControlModifier));
    m_basicmodul->m_memCalbtn = true;
    m_basicmodul->findChild<InputEdit *>()->setText("1");
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::ControlModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    m_basicmodul->handleEditKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    ASSERT_EQ(m_basicmodul->findChild<InputEdit *>()->text(), "1");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
    //25.2
}

TEST_F(Ut_BasicModul, handleKeypadButtonPress)
{
    BasicModule *m_basicmodul = new BasicModule;
//    QTest::mouseClick(m_basicmodul->findChild<BasicKeypad *>()->button(BasicKeypad::Key_Clear), Qt::LeftButton);
//    QTest::mouseClick(m_basicmodul->findChild<BasicKeypad *>()->button(BasicKeypad::Key_1), Qt::LeftButton);
//    QTest::mouseClick(m_basicmodul->findChild<BasicKeypad *>()->button(BasicKeypad::Key_Plus), Qt::LeftButton);
//    QTest::mouseClick(m_basicmodul->findChild<BasicKeypad *>()->button(BasicKeypad::Key_3), Qt::LeftButton);
//    QTest::mouseClick(m_basicmodul->findChild<BasicKeypad *>()->button(BasicKeypad::Key_Equals), Qt::LeftButton);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_0);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_1);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_2);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_3);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_4);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_5);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_6);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_7);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_8);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_9);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Plus);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Min);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Mult);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Div);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Percent);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Equals);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Clear);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Backspace);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Point);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Brackets);
    m_basicmodul->handleKeypadButtonPress(MemoryKeypad::Key_MS);
    m_basicmodul->handleKeypadButtonPress(MemoryKeypad::Key_MC);
    m_basicmodul->handleKeypadButtonPress(MemoryKeypad::Key_Mlist);
    m_basicmodul->handleKeypadButtonPress(MemoryKeypad::Key_Mplus);
    m_basicmodul->handleKeypadButtonPress(MemoryKeypad::Key_Mminus);
    m_basicmodul->handleKeypadButtonPress(MemoryKeypad::Key_MR);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_Clear);
    m_basicmodul->handleKeypadButtonPress(BasicKeypad::Key_1);
    ASSERT_EQ(m_basicmodul->findChild<InputEdit *>()->text(), "1");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, handleKeypadButtonPressByspace)
{
    BasicModule *m_basicmodul = new BasicModule;
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_0);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_1);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_2);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_3);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_4);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_5);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_6);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_7);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_8);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_9);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Plus);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Min);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Mult);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Div);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Percent);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Equals);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Clear);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Backspace);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Point);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Brackets);
    m_basicmodul->handleKeypadButtonPressByspace(MemoryKeypad::Key_MS);
    m_basicmodul->handleKeypadButtonPressByspace(MemoryKeypad::Key_MC);
    m_basicmodul->handleKeypadButtonPressByspace(MemoryKeypad::Key_Mlist);
    m_basicmodul->handleKeypadButtonPressByspace(MemoryKeypad::Key_Mplus);
    m_basicmodul->handleKeypadButtonPressByspace(MemoryKeypad::Key_Mminus);
    m_basicmodul->handleKeypadButtonPressByspace(MemoryKeypad::Key_MR);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_Clear);
    m_basicmodul->handleKeypadButtonPressByspace(BasicKeypad::Key_1);
    ASSERT_EQ(m_basicmodul->findChild<InputEdit *>()->text(), "1");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, mousePressEvent)
{
    BasicModule *m_basicmodul = new BasicModule;
    QTest::mouseClick(m_basicmodul->findChild<BasicKeypad *>()->button(BasicKeypad::Key_1), Qt::LeftButton);
    QTest::mouseClick(m_basicmodul->findChild<MemoryKeypad *>()->button(MemoryKeypad::Key_MS), Qt::LeftButton);
    QTest::mouseClick(m_basicmodul->findChild<MemoryKeypad *>()->button(MemoryKeypad::Key_Mlist), Qt::LeftButton);
    m_basicmodul->mousePressEvent(new QMouseEvent(QEvent::MouseButtonPress, m_basicmodul->findChild<InputEdit *>()->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
    QTest::mouseClick(m_basicmodul->findChild<MemoryKeypad *>()->button(MemoryKeypad::Key_Mlist), Qt::LeftButton);
    QTest::mouseClick(m_basicmodul->findChild<MemoryWidget *>()->findChild<IconButton *>(), Qt::LeftButton);
    m_basicmodul->mousePressEvent(new QMouseEvent(QEvent::MouseButtonPress, m_basicmodul->findChild<InputEdit *>()->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
    ASSERT_EQ(m_basicmodul->m_keypadLayout->currentIndex(), 0);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicModul, itemclick)
{
    BasicModule *m_basicmodul = new BasicModule;
    QTest::mouseClick(m_basicmodul->findChild<BasicKeypad *>()->button(BasicKeypad::Key_1), Qt::LeftButton);
    QTest::mouseClick(m_basicmodul->findChild<MemoryKeypad *>()->button(MemoryKeypad::Key_MS), Qt::LeftButton);
    QTest::mouseClick(m_basicmodul->findChild<MemoryKeypad *>()->button(MemoryKeypad::Key_Mlist), Qt::LeftButton);
    QTest::mouseClick(m_basicmodul->findChild<MemoryItemWidget *>(), Qt::LeftButton);
    ASSERT_EQ(m_basicmodul->m_keypadLayout->currentIndex(), 0);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}
