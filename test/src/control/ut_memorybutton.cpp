#include "ut_memorybutton.h"
#include "../../src/control/memorybutton.h"

Ut_MemoryButton::Ut_MemoryButton()
{

}

TEST_F(Ut_MemoryButton, initanimate)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->m_widgetbtn = false;
    m_memorybutton->animate(false, 100);
    m_memorybutton->init();
    ASSERT_EQ(m_memorybutton->m_font.pixelSize(), 16);
}

TEST_F(Ut_MemoryButton, setbtnlight)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    DSettingsAlt *m_dsetting = new DSettingsAlt;
    m_memorybutton->setbtnlight(true);
    m_memorybutton->setbtnlight(false);
    if (m_dsetting->getOption("mode") == 1)
        ASSERT_EQ(m_memorybutton->text(), "MH˅");
    else
        ASSERT_EQ(m_memorybutton->text(), "M˅");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_MemoryButton, showtips)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->setText("M+");
    m_memorybutton->showtips();
    m_memorybutton->setText("MR");
    m_memorybutton->showtips();
    m_memorybutton->setText("M-");
    m_memorybutton->showtips();
    m_memorybutton->setText("MS");
    m_memorybutton->showtips();
    m_memorybutton->setText("MC");
    m_memorybutton->m_widgetbtn = true;
    m_memorybutton->showtips();
    ASSERT_EQ(m_memorybutton->toolTip(), "Clear memory item");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_MemoryButton, setbuttongray)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->setbuttongray(true);
    ASSERT_TRUE(m_memorybutton->m_isallgray);
}

TEST_F(Ut_MemoryButton, mousePressEvent)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                    m_memorybutton->pos(), Qt::MouseButton::LeftButton,
                                                    Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_TRUE(m_memorybutton->m_isPress);
}

TEST_F(Ut_MemoryButton, mouseReleaseEvent)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                                      m_memorybutton->pos(), Qt::MouseButton::LeftButton,
                                                      Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_memorybutton->m_isPress);
}

TEST_F(Ut_MemoryButton, enterEvent)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->enterEvent(new QEvent(QEvent::Type::Enter));
    ASSERT_TRUE(m_memorybutton->m_isHover);
}

TEST_F(Ut_MemoryButton, leaveEvent)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->leaveEvent(new QEvent(QEvent::Type::Leave));
    ASSERT_FALSE(m_memorybutton->m_isHover);
}

TEST_F(Ut_MemoryButton, focusOutEvent)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    m_memorybutton->focusOutEvent(new QFocusEvent(QEvent::Type::FocusOut));
    //无ASSERT
}

bool stub_focus_memory()
{
    return true;
}

TEST_F(Ut_MemoryButton, paintEvent)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    QPaintEvent *event = new QPaintEvent(m_memorybutton->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_memorybutton->setText("MH˅");
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = true;
    m_memorybutton->m_isPress = false;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_memorybutton->setEnabled(false);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = false;
    m_memorybutton->m_isgray = true;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = true;
    m_memorybutton->paintEvent(event);

    //focus状态
    Stub stub;
    stub.set(ADDR(MemoryButton, hasFocus), stub_focus_memory);
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = true;
    m_memorybutton->m_isPress = false;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = true;
    m_memorybutton->paintEvent(event);
    //无ASSERT
}

TEST_F(Ut_MemoryButton, paintEvent2)
{
    MemoryButton *m_memorybutton = new MemoryButton;
    QPaintEvent *event = new QPaintEvent(m_memorybutton->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_memorybutton->setText("MH˄");
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = true;
    m_memorybutton->m_isPress = false;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_memorybutton->setEnabled(false);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = false;
    m_memorybutton->m_isgray = true;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = true;
    m_memorybutton->paintEvent(event);

    //focus状态
    Stub stub;
    stub.set(ADDR(MemoryButton, hasFocus), stub_focus_memory);
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = true;
    m_memorybutton->m_isPress = false;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = true;
    m_memorybutton->paintEvent(event);
    //无ASSERT
}

TEST_F(Ut_MemoryButton, paintEvent3)
{
    MemoryButton *m_memorybutton = new MemoryButton(QString("M+"), true);
    QPaintEvent *event = new QPaintEvent(m_memorybutton->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = true;
    m_memorybutton->m_isPress = false;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_memorybutton->setEnabled(false);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = false;
    m_memorybutton->m_isgray = true;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = true;
    m_memorybutton->paintEvent(event);

    //focus状态
    Stub stub;
    stub.set(ADDR(MemoryButton, hasFocus), stub_focus_memory);
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = true;
    m_memorybutton->m_isPress = false;
    m_memorybutton->paintEvent(event);
    m_memorybutton->update();
    m_memorybutton->setEnabled(true);
    m_memorybutton->m_isHover = false;
    m_memorybutton->m_isPress = true;
    m_memorybutton->paintEvent(event);
    //无ASSERT
}
