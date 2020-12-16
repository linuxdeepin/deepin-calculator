#include "ut_programmerkeypad.h"
#include "../../src/control/programmerkeypad.h"


Ut_ProgrammerKeypad::Ut_ProgrammerKeypad()
{

}

TEST_F(Ut_ProgrammerKeypad, button)
{
    ProgrammerKeypad *m_programmerkeypad = new ProgrammerKeypad;
    ASSERT_EQ(m_programmerkeypad->button(ProgrammerKeypad::Buttons::Key_AND)->text(), "AND");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProgrammerKeypad, buttonHasFocus)
{
    ProgrammerKeypad *m_programmerkeypad = new ProgrammerKeypad;
    ASSERT_FALSE(m_programmerkeypad->buttonHasFocus());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProgrammerKeypad, initButtons)
{
    ProgrammerKeypad *m_programmerkeypad = new ProgrammerKeypad;
    m_programmerkeypad->initButtons();
    ASSERT_EQ(m_programmerkeypad->m_keys.size(), 36);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProgrammerKeypad, getFocus)
{
    ProgrammerKeypad *m_programmerkeypad = new ProgrammerKeypad;
    m_programmerkeypad->getFocus(1);
    m_programmerkeypad->getFocus(2);
    m_programmerkeypad->getFocus(3);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProgrammerKeypad, radixChanged)
{
    ProgrammerKeypad *m_programmerkeypad = new ProgrammerKeypad;
    m_programmerkeypad->radixChanged(0);
    m_programmerkeypad->radixChanged(1);
    m_programmerkeypad->radixChanged(2);
    m_programmerkeypad->radixChanged(3);
    m_programmerkeypad->radixChanged(5);
    ASSERT_EQ(m_programmerkeypad->m_currentBase, 10);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProgrammerKeypad, bracketsNum)
{
    ProgrammerKeypad *m_programmerkeypad = new ProgrammerKeypad;
    m_programmerkeypad->bracketsNum(0, "0");
    m_programmerkeypad->bracketsNum(1, "1");
    ASSERT_EQ(m_programmerkeypad->m_rightBracket->text(), "1");
    DSettingsAlt::deleteInstance();
}

