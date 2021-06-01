#include "ut_programmeritemwidget.h"
#include "../../src/views/programmeritemwidget.h"
#include "../../src/dsettings.h"
#include "../../src/memorypublic.h"


Ut_ProgrammerItemWidget::Ut_ProgrammerItemWidget()
{

}

TEST_F(Ut_ProgrammerItemWidget, enterEvent)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->enterEvent(new QEvent(QEvent::Type::MouseMove));
    ASSERT_TRUE(m_programmerItemWidget->m_ishover);
}

TEST_F(Ut_ProgrammerItemWidget, leaveEvent)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->leaveEvent(new QEvent(QEvent::Type::MouseMove));
    ASSERT_FALSE(m_programmerItemWidget->m_ishover);
}

TEST_F(Ut_ProgrammerItemWidget, mousePressEvent)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                            m_programmerItemWidget->pos(), Qt::MouseButton::RightButton,
                                                            Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_programmerItemWidget->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                            m_programmerItemWidget->pos(), Qt::MouseButton::LeftButton,
                                                            Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_TRUE(m_programmerItemWidget->m_ispress);
}

TEST_F(Ut_ProgrammerItemWidget, mouseReleaseEvent)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                              m_programmerItemWidget->pos(), Qt::MouseButton::RightButton,
                                                              Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_programmerItemWidget->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                                              m_programmerItemWidget->pos(), Qt::MouseButton::LeftButton,
                                                              Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_programmerItemWidget->m_ispress);
}

TEST_F(Ut_ProgrammerItemWidget, cleanHoverState)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->cleanHoverState();
    ASSERT_FALSE(m_programmerItemWidget->m_ishover);
}

TEST_F(Ut_ProgrammerItemWidget, cleanFocusState)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->cleanFocusState();
    ASSERT_FALSE(m_programmerItemWidget->m_isfocus);
}

TEST_F(Ut_ProgrammerItemWidget, isMarkHide)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->isMarkHide(true);
    ASSERT_TRUE(m_programmerItemWidget->m_isMarkHide);
}

TEST_F(Ut_ProgrammerItemWidget, initMark)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->initMark();
    ASSERT_EQ(m_programmerItemWidget->m_pointVec.at(1), m_programmerItemWidget->m_markmid);
}

TEST_F(Ut_ProgrammerItemWidget, themetypechanged)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->themetypechanged(2);
    m_programmerItemWidget->themetypechanged(1);
    ASSERT_EQ(m_programmerItemWidget->m_label->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text).name(),
              "#000000");
}

TEST_F(Ut_ProgrammerItemWidget, setFocus)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->setFocus();
    ASSERT_TRUE(m_programmerItemWidget->m_isfocus);
}

TEST_F(Ut_ProgrammerItemWidget, paintEvent)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    QPaintEvent *event = new QPaintEvent(m_programmerItemWidget->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_programmerItemWidget->m_ispress = true;
    m_programmerItemWidget->m_ishover = false;
    m_programmerItemWidget->paintEvent(event);
    m_programmerItemWidget->update();
    ProgrammerItemWidget *m_programmerItemWidget1 = new ProgrammerItemWidget("Circular shift", 50, ":/assets/images/dark/icon_as_normal.svg");
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_programmerItemWidget1->m_isMarkHide = false;
    m_programmerItemWidget1->m_isshift = true;
    m_programmerItemWidget1->m_ispress = false;
    m_programmerItemWidget1->m_ishover = true;
    m_programmerItemWidget1->paintEvent(event);
    m_programmerItemWidget1->update();
}
