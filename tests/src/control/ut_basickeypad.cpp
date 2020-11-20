#include "ut_basickeypad.h"
#include "../../src/control/basickeypad.h"
#include "../../src/memorypublic.h"

Ut_BasicKeypad::Ut_BasicKeypad()
{

}

TEST_F(Ut_BasicKeypad, buttonThemeChanged)
{
    BasicKeypad *m_basickeypad = new BasicKeypad;
    m_basickeypad->buttonThemeChanged(1);
    m_basickeypad->buttonThemeChanged(2);
    ASSERT_EQ(static_cast<IconButton *>(m_basickeypad->button(BasicKeypad::Key_Div))->m_currentUrl, ":/assets/images/dark/divide_normal.svg");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_BasicKeypad, getFocus)
{
    BasicKeypad *m_basickeypad = new BasicKeypad;
    m_basickeypad->getFocus(0);
    m_basickeypad->getFocus(1);
    m_basickeypad->getFocus(2);
    m_basickeypad->getFocus(3);
    m_basickeypad->getFocus(4);
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}
