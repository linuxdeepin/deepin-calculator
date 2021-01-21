#include "ut_iconbutton.h"
#include "../../src/control/iconbutton.h"

Ut_IconButton::Ut_IconButton()
{

}

TEST_F(Ut_IconButton, setIconUrl)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->setIconUrl("a", "b", "c", 2);
    ASSERT_EQ(m_iconButton->m_mode, 2);
}

TEST_F(Ut_IconButton, showtooltip)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->showtooltip(false);
    m_iconButton->showtooltip(true);
    ASSERT_EQ(m_iconButton->m_cleartooltip, "Clear all memory");
}

TEST_F(Ut_IconButton, updateWhenBtnDisable)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->updateWhenBtnDisable();
    ASSERT_FALSE(m_iconButton->m_isPress);
}

TEST_F(Ut_IconButton, setBtnPressing)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->setBtnPressing(false);
    m_iconButton->setBtnPressing(true);
    ASSERT_TRUE(m_iconButton->m_isPressing);
}

TEST_F(Ut_IconButton, setBtnHighlight)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->setBtnHighlight(false);
    ASSERT_FALSE(m_iconButton->m_highlight);
}

TEST_F(Ut_IconButton, mousePressEvent)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->animate(false, 100);
    m_iconButton->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                  m_iconButton->pos(), Qt::MouseButton::LeftButton,
                                                  Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_TRUE(m_iconButton->m_isPress);
}

TEST_F(Ut_IconButton, mouseReleaseEvent)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->animate(false, 100);
    m_iconButton->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                                    m_iconButton->pos(), Qt::MouseButton::LeftButton,
                                                    Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_iconButton->m_isPress);
}

TEST_F(Ut_IconButton, enterEvent)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->enterEvent(new QEvent(QEvent::Type::Enter));
    ASSERT_TRUE(m_iconButton->m_isHover);
}

TEST_F(Ut_IconButton, leaveEvent)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->leaveEvent(new QEvent(QEvent::Type::Leave));
    ASSERT_FALSE(m_iconButton->m_isHover);
}

bool stub_focus_icon()
{
    return true;
}

TEST_F(Ut_IconButton, paintEvent)
{
    IconButton *m_iconButton = new IconButton;
    QPaintEvent *event = new QPaintEvent(m_iconButton->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_iconButton->m_isHover = true;
    m_iconButton->m_isPress = false;
    m_iconButton->m_isEmptyBtn = false;
    m_iconButton->paintEvent(event);
    m_iconButton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_iconButton->m_isHover = false;
    m_iconButton->m_isPress = true;
    m_iconButton->m_isEmptyBtn = false;
    m_iconButton->paintEvent(event);
    m_iconButton->update();
    m_iconButton->m_isHover = false;
    m_iconButton->m_isPress = false;
    m_iconButton->m_isEmptyBtn = true;
    m_iconButton->paintEvent(event);

    //SetAttrRecur
    m_iconButton->m_mode = 2;
    m_iconButton->m_buttonStatus = 2;
    m_iconButton->m_currentUrl = ":/assets/images/light/clear_press.svg";
    m_iconButton->paintEvent(event);
    m_iconButton->m_mode = 0;
    m_iconButton->m_buttonStatus = 0;
    m_iconButton->m_currentUrl = ":/assets/images/light/+_press.svg";
    m_iconButton->paintEvent(event);
    m_iconButton->m_buttonStatus = 1;
    m_iconButton->paintEvent(event);
    m_iconButton->m_mode = 4;
    m_iconButton->m_buttonStatus = 2;
    m_iconButton->m_currentUrl = ":/assets/images/light/squareroot_press.svg";
    m_iconButton->paintEvent(event);
    m_iconButton->m_mode = 6;
    m_iconButton->m_currentUrl = ":/assets/images/light/deg_press.svg";
    m_iconButton->paintEvent(event);

    //focus状态
    Stub stub;
    stub.set(ADDR(IconButton, hasFocus), stub_focus_icon);
    m_iconButton->setEnabled(true);
    m_iconButton->m_isHover = true;
    m_iconButton->m_isPress = false;
    m_iconButton->paintEvent(event);
    m_iconButton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_iconButton->m_isHover = false;
    m_iconButton->m_isPress = true;
    m_iconButton->m_isEmptyBtn = false;
    m_iconButton->paintEvent(event);
    m_iconButton->update();
    m_iconButton->m_isHover = false;
    m_iconButton->m_isPress = false;
    m_iconButton->m_isEmptyBtn = true;
    m_iconButton->paintEvent(event);

    //SetAttrRecur
    m_iconButton->m_mode = 2;
    m_iconButton->m_buttonStatus = 2;
    m_iconButton->m_currentUrl = ":/assets/images/light/clear_press.svg";
    m_iconButton->paintEvent(event);
    m_iconButton->m_mode = 0;
    m_iconButton->m_buttonStatus = 0;
    m_iconButton->m_currentUrl = ":/assets/images/light/+_press.svg";
    m_iconButton->paintEvent(event);
    m_iconButton->m_buttonStatus = 1;
    m_iconButton->paintEvent(event);
    m_iconButton->m_mode = 4;
    m_iconButton->m_buttonStatus = 2;
    m_iconButton->m_currentUrl = ":/assets/images/light/squareroot_press.svg";
    m_iconButton->paintEvent(event);
    m_iconButton->m_mode = 6;
    m_iconButton->m_currentUrl = ":/assets/images/light/deg_press.svg";
    m_iconButton->paintEvent(event);
    //无ASSERT
}

TEST_F(Ut_IconButton, keyPressEvent)
{
    IconButton *m_iconButton = new IconButton;
    m_iconButton->m_isEmptyBtn = true;
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
    m_iconButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
    //无ASSERT
}
