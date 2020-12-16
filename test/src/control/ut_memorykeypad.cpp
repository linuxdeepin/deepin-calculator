#include "ut_memorykeypad.h"
#include "../../src/control/memorykeypad.h"

Ut_MemoryKeypad::Ut_MemoryKeypad()
{

}

TEST_F(Ut_MemoryKeypad, button)
{
    MemoryKeypad *m_memorykeypad = new MemoryKeypad;
    ASSERT_EQ(m_memorykeypad->button(MemoryKeypad::Buttons::Key_MC)->text(), "MC");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_MemoryKeypad, getFocus)
{
    MemoryKeypad *m_basickeypad = new MemoryKeypad;
    m_basickeypad->getFocus(1);
    m_basickeypad->getFocus(2);
    m_basickeypad->getFocus(3);
    //无ASSERT
    DSettingsAlt::deleteInstance();
}
