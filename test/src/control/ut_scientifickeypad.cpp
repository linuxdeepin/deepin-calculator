#include "ut_scientifickeypad.h"
#include "../../src/control/scientifickeypad.h"
#include "../../src/memorypublic.h"

Ut_ScientificKeypad::Ut_ScientificKeypad()
{

}

TEST_F(Ut_ScientificKeypad, button1)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->button(ScientificKeyPad::Key_arcsin);
    ASSERT_EQ(m_scientifickeypad->button(ScientificKeyPad::Key_sin)->text(), "sin");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, button2)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->button(42);
    ASSERT_EQ(m_scientifickeypad->button(6)->text(), "sin");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, animate)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->animate(ScientificKeyPad::Key_sin, false);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, buttonHasFocus)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->button(6)->setFocus();
    ASSERT_FALSE(m_scientifickeypad->buttonHasFocus());
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, initButtons)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->initButtons();
    ASSERT_EQ(m_scientifickeypad->m_keys.size(), 42);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, LeftButton)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->m_themetype = 1;
    QTest::mouseRelease(m_scientifickeypad->button(ScientificKeyPad::Key_left), Qt::LeftButton);
    m_scientifickeypad->m_themetype = 2;
    QTest::mouseRelease(m_scientifickeypad->button(ScientificKeyPad::Key_left), Qt::LeftButton);
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, buttonThemeChanged)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->m_deg = 3;
    m_scientifickeypad->buttonThemeChanged(1);
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, turnPage)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->turnPage(ScientificKeyPad::Key_page);
    m_scientifickeypad->turnPage(ScientificKeyPad::Key_page);
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, getdeg)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->getdeg(1);
    m_scientifickeypad->getdeg(2);
    m_scientifickeypad->getdeg(3);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, bracketsNum)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->bracketsNum(0, "0");
    m_scientifickeypad->bracketsNum(1, "1");
    ASSERT_EQ(m_scientifickeypad->m_rightBracket->text(), "1");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, getFocus)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->getFocus(0);
    m_scientifickeypad->getFocus(1);
    m_scientifickeypad->getFocus(2);
    m_scientifickeypad->getFocus(3);
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

