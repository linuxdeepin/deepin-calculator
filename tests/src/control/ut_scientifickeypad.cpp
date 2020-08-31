#include "ut_scientifickeypad.h"
#define private public
#include "src/control/scientifickeypad.h"
#include "src/memorypublic.h"
#undef private

Ut_ScientificKeypad::Ut_ScientificKeypad()
{

}

TEST_F(Ut_ScientificKeypad, LeftButton)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->m_themetype = 1;
    QTest::mouseRelease(m_scientifickeypad->button(ScientificKeyPad::Key_left), Qt::LeftButton);
    m_scientifickeypad->m_themetype = 2;
    QTest::mouseRelease(m_scientifickeypad->button(ScientificKeyPad::Key_left), Qt::LeftButton);
    //无ASSERT
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, buttonThemeChanged)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->m_deg = 3;
    m_scientifickeypad->buttonThemeChanged(1);
    //无ASSERT
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, turnPage)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->turnPage(ScientificKeyPad::Key_page);
    m_scientifickeypad->turnPage(ScientificKeyPad::Key_page);
    //无ASSERT
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ScientificKeypad, getFocus)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
    m_scientifickeypad->getFocus(0);
    m_scientifickeypad->getFocus(1);
    m_scientifickeypad->getFocus(2);
    m_scientifickeypad->getFocus(3);
    //无ASSERT
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

