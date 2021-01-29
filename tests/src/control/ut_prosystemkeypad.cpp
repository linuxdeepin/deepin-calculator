#include "ut_prosystemkeypad.h"
#include "../../src/control/prosystemkeypad.h"

Ut_ProSystemKeypad::Ut_ProSystemKeypad()
{

}

TEST_F(Ut_ProSystemKeypad, buttonHasFocus)
{
    ProSystemKeypad *m_prosystemkeypad = new ProSystemKeypad;
    ASSERT_FALSE(m_prosystemkeypad->buttonHasFocus());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProSystemKeypad, getFocus)
{
    ProSystemKeypad *m_prosystemkeypad = new ProSystemKeypad;
    m_prosystemkeypad->getFocus(1, 1);
    m_prosystemkeypad->getFocus(2, 1);
    m_prosystemkeypad->getFocus(3, 1);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProSystemKeypad, setSystem)
{
    ProSystemKeypad *m_prosystemkeypad = new ProSystemKeypad;
    m_prosystemkeypad->setSystem(32, 64);
    m_prosystemkeypad->setSystem(64, 8);
    m_prosystemkeypad->setSystem(8, 16);
    m_prosystemkeypad->setSystem(16, 32);
    m_prosystemkeypad->setSystem(64, 32);
    m_prosystemkeypad->setSystem(32, 16);
    m_prosystemkeypad->setSystem(16, 8);
    m_prosystemkeypad->setSystem(8, 64);
    ASSERT_FALSE(m_prosystemkeypad->m_buttons.value(60)->isEnabled());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProSystemKeypad, setvalue)
{
    ProSystemKeypad *m_prosystemkeypad = new ProSystemKeypad;
    m_prosystemkeypad->setvalue("101111111");
    ASSERT_EQ(m_prosystemkeypad->m_binaryValue, "0000000000000000000000000000000000000000000000000000000101111111");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProSystemKeypad, longBitCut)
{
    ProSystemKeypad *m_prosystemkeypad = new ProSystemKeypad;
    m_prosystemkeypad->longBitCut("110000000000000000000000000000000000000000000000000000000101111111");
    ASSERT_EQ(m_prosystemkeypad->m_binaryValue, "0000000000000000000000000000000000000000000000000000000000000000");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProSystemKeypad, changeBinaryValue)
{
    ProSystemKeypad *m_prosystemkeypad = new ProSystemKeypad;
    m_prosystemkeypad->changeBinaryValue(5);
    ASSERT_EQ(m_prosystemkeypad->m_binaryValue, "0000000000000000000000000000000000000000000000000000000000100000");
    DSettingsAlt::deleteInstance();
}
