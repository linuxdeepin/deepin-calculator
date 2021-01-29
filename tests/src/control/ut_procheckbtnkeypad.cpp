#include "ut_procheckbtnkeypad.h"
#include "../../src/control/procheckbtnkeypad.h"

Ut_ProCheckBtnKeypad::Ut_ProCheckBtnKeypad()
{

}

TEST_F(Ut_ProCheckBtnKeypad, button)
{
    ProCheckBtnKeypad *m_procheckbtnkeypad = new ProCheckBtnKeypad;
    ASSERT_EQ(m_procheckbtnkeypad->button(ProCheckBtnKeypad::Buttons::Key_MS)->text(), "MS");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProCheckBtnKeypad, animate)
{
    ProCheckBtnKeypad *m_procheckbtnkeypad = new ProCheckBtnKeypad;
    m_procheckbtnkeypad->animate(ProCheckBtnKeypad::Buttons::Key_MS, false);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProCheckBtnKeypad, buttonHasFocus)
{
    ProCheckBtnKeypad *m_procheckbtnkeypad = new ProCheckBtnKeypad;
    ASSERT_FALSE(m_procheckbtnkeypad->buttonHasFocus());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProCheckBtnKeypad, initButtons)
{
    ProCheckBtnKeypad *m_procheckbtnkeypad = new ProCheckBtnKeypad;
    m_procheckbtnkeypad->initButtons();
    ASSERT_EQ(m_procheckbtnkeypad->m_keys.size(), 6);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProCheckBtnKeypad, buttonThemeChanged)
{
    ProCheckBtnKeypad *m_procheckbtnkeypad = new ProCheckBtnKeypad;
    m_procheckbtnkeypad->buttonThemeChanged(1);
    m_procheckbtnkeypad->buttonThemeChanged(2);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProCheckBtnKeypad, getFocus)
{
    ProCheckBtnKeypad *m_procheckbtnkeypad = new ProCheckBtnKeypad;
    m_procheckbtnkeypad->getFocus(1);
    m_procheckbtnkeypad->getFocus(2);
    m_procheckbtnkeypad->getFocus(3);
    DSettingsAlt::deleteInstance();
}
