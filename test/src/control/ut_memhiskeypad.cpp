#include "ut_memhiskeypad.h"
#include "../../src/control/memhiskeypad.h"

Ut_MemHisKeypad::Ut_MemHisKeypad()
{

}

TEST_F(Ut_MemHisKeypad, button)
{
    MemHisKeypad *m_memhiskeypad = new MemHisKeypad;
    ASSERT_EQ(m_memhiskeypad->button(MemHisKeypad::Buttons::Key_MC)->text(), "MC");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_MemHisKeypad, animate)
{
    MemHisKeypad *m_memhiskeypad = new MemHisKeypad;
    m_memhiskeypad->animate(MemHisKeypad::Buttons::Key_MC, false);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_MemHisKeypad, buttonHasFocus)
{
    MemHisKeypad *m_memhiskeypad = new MemHisKeypad;
    ASSERT_FALSE(m_memhiskeypad->buttonHasFocus());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_MemHisKeypad, initButtons)
{
    MemHisKeypad *m_memhiskeypad = new MemHisKeypad;
    m_memhiskeypad->initButtons();
    ASSERT_EQ(m_memhiskeypad->m_keys.size(), 7);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_MemHisKeypad, getFocus)
{
    MemHisKeypad *m_memhiskeypad = new MemHisKeypad;
    m_memhiskeypad->getFocus(1);
    m_memhiskeypad->getFocus(2);
    m_memhiskeypad->getFocus(3);
    DSettingsAlt::deleteInstance();
}
