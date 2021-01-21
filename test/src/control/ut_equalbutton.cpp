#include "ut_equalbutton.h"
#include "../../src/control/equalbutton.h"

Ut_EqualButton::Ut_EqualButton()
{

}

TEST_F(Ut_EqualButton, init)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->init();
    ASSERT_EQ(m_equalButton->m_font.pixelSize(), 36);
}

TEST_F(Ut_EqualButton, mousePressEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                   m_equalButton->pos(), Qt::MouseButton::LeftButton,
                                                   Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_TRUE(m_equalButton->m_isPress);
}

TEST_F(Ut_EqualButton, mouseReleaseEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                                     m_equalButton->pos(), Qt::MouseButton::LeftButton,
                                                     Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_equalButton->m_isPress);
}

TEST_F(Ut_EqualButton, enterEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->enterEvent(new QEvent(QEvent::Type::Enter));
    ASSERT_TRUE(m_equalButton->m_isHover);
}

TEST_F(Ut_EqualButton, leaveEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->leaveEvent(new QEvent(QEvent::Type::Leave));
    ASSERT_FALSE(m_equalButton->m_isHover);
}

bool stub_focus_equal()
{
    return true;
}

TEST_F(Ut_EqualButton, paintEvent)
{
    EqualButton *m_equalButton = new EqualButton();
    QPaintEvent *event = new QPaintEvent(m_equalButton->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_equalButton->setEnabled(true);
    m_equalButton->m_isHover = true;
    m_equalButton->m_isPress = false;
    m_equalButton->paintEvent(event);
    m_equalButton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_equalButton->paintEvent(event);
    m_equalButton->m_isHover = false;
    m_equalButton->m_isPress = true;
    m_equalButton->paintEvent(event);
    m_equalButton->update();
    m_equalButton->m_isHover = false;
    m_equalButton->m_isPress = false;
    m_equalButton->paintEvent(event);
    m_equalButton->update();

    //focus状态
    Stub stub;
    stub.set(ADDR(EqualButton, hasFocus), stub_focus_equal);
    m_equalButton->m_isHover = true;
    m_equalButton->m_isPress = false;
    m_equalButton->paintEvent(event);
    m_equalButton->update();
    m_equalButton->paintEvent(event);
    m_equalButton->m_isHover = false;
    m_equalButton->m_isPress = true;
    m_equalButton->paintEvent(event);
    m_equalButton->update();
    m_equalButton->m_isHover = false;
    m_equalButton->m_isPress = false;
    m_equalButton->paintEvent(event);
    m_equalButton->update();
    //无ASSERT
}

TEST_F(Ut_EqualButton, keyPressEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    m_equalButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
    m_equalButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_equalButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_equalButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    m_equalButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    //无ASSERT
}
