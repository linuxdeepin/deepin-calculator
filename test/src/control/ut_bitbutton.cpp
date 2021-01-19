#include "ut_bitbutton.h"
#include "../../src/control/bitbutton.h"

Ut_BitButton::Ut_BitButton()
{

}

TEST_F(Ut_BitButton, init)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->init();
    ASSERT_EQ(m_bitbutton->m_font.pixelSize(), 18);
}

TEST_F(Ut_BitButton, animate)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->m_isHover = true;
    m_bitbutton->m_isPress = false;
    m_bitbutton->animate(false, 100);
    ASSERT_FALSE(m_bitbutton->m_isHover);
}

TEST_F(Ut_BitButton, setButtonState)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->setButtonState(false);
    m_bitbutton->setButtonState(true);
    ASSERT_EQ(m_bitbutton->text(), "1");
}

TEST_F(Ut_BitButton, mousePressEvent)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                 m_bitbutton->pos(), Qt::MouseButton::LeftButton,
                                                 Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_TRUE(m_bitbutton->m_isPress);
}

TEST_F(Ut_BitButton, mouseReleaseEvent)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                                   m_bitbutton->pos(), Qt::MouseButton::LeftButton,
                                                   Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_bitbutton->m_isPress);
}

TEST_F(Ut_BitButton, enterEvent)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->enterEvent(new QEvent(QEvent::Type::Enter));
    ASSERT_TRUE(m_bitbutton->m_isHover);
}

TEST_F(Ut_BitButton, leaveEvent)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->leaveEvent(new QEvent(QEvent::Type::Leave));
    ASSERT_FALSE(m_bitbutton->m_isHover);
}

TEST_F(Ut_BitButton, keyPressEvent)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
    m_bitbutton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    //无ASSERT
}

TEST_F(Ut_BitButton, focusOutEvent)
{
    BitButton *m_bitbutton = new BitButton;
    m_bitbutton->focusOutEvent(new QFocusEvent(QEvent::Type::FocusOut));
}

TEST_F(Ut_BitButton, paintEvent)
{
    BitButton *m_bitbutton = new BitButton;
    QPaintEvent *event = new QPaintEvent(m_bitbutton->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_bitbutton->m_isHover = true;
    m_bitbutton->m_isPress = true;
    m_bitbutton->paintEvent(event);
    m_bitbutton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_bitbutton->paintEvent(event);
    //无ASSERT
}
