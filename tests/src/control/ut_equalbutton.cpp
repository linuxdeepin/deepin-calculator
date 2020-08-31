#include "ut_equalbutton.h"
#define private public
#include "src/control/equalbutton.h"
#undef private

Ut_EqualButton::Ut_EqualButton()
{

}

TEST_F(Ut_EqualButton, mousePressEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                   m_equalButton->pos(), Qt::MouseButton::LeftButton,
                                                   Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_TRUE(m_equalButton->m_isPress);
    DSettings::deleteInstance();
}

TEST_F(Ut_EqualButton, mouseReleaseEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                                     m_equalButton->pos(), Qt::MouseButton::LeftButton,
                                                     Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_equalButton->m_isPress);
    DSettings::deleteInstance();
}

TEST_F(Ut_EqualButton, enterEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->enterEvent(new QEvent(QEvent::Type::Enter));
    ASSERT_TRUE(m_equalButton->m_isHover);
    DSettings::deleteInstance();
}

TEST_F(Ut_EqualButton, leaveEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    m_equalButton->leaveEvent(new QEvent(QEvent::Type::Leave));
    ASSERT_FALSE(m_equalButton->m_isHover);
    DSettings::deleteInstance();
}

TEST_F(Ut_EqualButton, paintEvent)
{
    EqualButton *m_equalButton = new EqualButton;
    QPaintEvent *event = new QPaintEvent(m_equalButton->rect());
    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_equalButton->m_isHover = true;
    m_equalButton->m_isPress = true;
    m_equalButton->paintEvent(event);
    m_equalButton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_equalButton->paintEvent(event);
    //无ASSERT
    DSettings::deleteInstance();
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
    DSettings::deleteInstance();
}
