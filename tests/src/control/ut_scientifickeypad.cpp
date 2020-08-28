#include "ut_scientifickeypad.h"
#define private public
#include "src/control/scientifickeypad.h"
#include "src/memorypublic.h"
#undef private

Ut_ScientificKeypad::Ut_ScientificKeypad()
{

}

TEST_F(Ut_ScientificKeypad, buttonThemeChanged)
{
    ScientificKeyPad *m_scientifickeypad = new ScientificKeyPad;
//    m_scientifickeypad->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
//                                                          static_cast<TextButton *>(m_scientifickeypad->button(ScientificKeyPad::Key_left))->pos(), Qt::MouseButton::LeftButton,
//                                                          Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_scientifickeypad->m_themetype = 1;
    QTest::mouseRelease(m_scientifickeypad->button(ScientificKeyPad::Key_left), Qt::LeftButton);
    m_scientifickeypad->m_themetype = 2;
    QTest::mouseRelease(m_scientifickeypad->button(ScientificKeyPad::Key_left), Qt::LeftButton);
    //    ASSERT_EQ(static_cast<IconButton *>(m_basickeypad->button(BasicKeypad::Key_Div))->m_currentUrl, ":/assets/images/dark/divide_normal.svg");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}
