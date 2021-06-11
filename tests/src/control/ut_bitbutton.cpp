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
    QMouseEvent *m = new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                     m_bitbutton->pos(), Qt::MouseButton::LeftButton,
                                     Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
    m_bitbutton->mousePressEvent(m);
    ASSERT_TRUE(m_bitbutton->m_isPress);
    delete m;
}

TEST_F(Ut_BitButton, mouseReleaseEvent)
{
    BitButton *m_bitbutton = new BitButton;
    QMouseEvent *m = new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                     m_bitbutton->pos(), Qt::MouseButton::LeftButton,
                                     Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
    m_bitbutton->mouseReleaseEvent(m);
    ASSERT_FALSE(m_bitbutton->m_isPress);
    delete m;
}

TEST_F(Ut_BitButton, enterEvent)
{
    BitButton *m_bitbutton = new BitButton;
    QEvent *q = new QEvent(QEvent::Type::Enter);

    m_bitbutton->enterEvent(q);
    ASSERT_TRUE(m_bitbutton->m_isHover);
    delete q;
}

TEST_F(Ut_BitButton, leaveEvent)
{
    BitButton *m_bitbutton = new BitButton;
    QEvent *q = new QEvent(QEvent::Type::Leave);

    m_bitbutton->leaveEvent(q);
    ASSERT_FALSE(m_bitbutton->m_isHover);
    delete q;
}

TEST_F(Ut_BitButton, keyPressEvent)
{
    BitButton *m_bitbutton = new BitButton;
    QKeyEvent *k = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    QKeyEvent *k1 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    QKeyEvent *k2 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    QKeyEvent *k3 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    QKeyEvent *k4 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
    QKeyEvent *k5 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
    m_bitbutton->keyPressEvent(k);
    m_bitbutton->keyPressEvent(k1);
    m_bitbutton->keyPressEvent(k2);
    m_bitbutton->keyPressEvent(k3);
    m_bitbutton->keyPressEvent(k4);
    m_bitbutton->keyPressEvent(k5);
    delete k;
    delete k1;
    delete k2;
    delete k3;
    delete k4;
    delete k5;
    //无ASSERT
}

TEST_F(Ut_BitButton, focusOutEvent)
{
    BitButton *m_bitbutton = new BitButton;
    QFocusEvent *f = new QFocusEvent(QEvent::Type::FocusOut);
    m_bitbutton->focusOutEvent(f);
    delete f;
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
