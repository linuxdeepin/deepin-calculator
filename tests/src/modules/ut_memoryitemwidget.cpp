#include "ut_memoryitemwidget.h"
#define private public
#include "src/modules/memoryitemwidget.h"
#undef private
#include "src/dsettings.h"
#include "src/modules/memorypublic.h"

Ut_MemoryItemWidget::Ut_MemoryItemWidget()
{

}

TEST_F(Ut_MemoryItemWidget, enterEvent)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->enterEvent(new QEvent(QEvent::Type::MouseMove));
    ASSERT_TRUE(m_memoryItemWidget->m_ishover);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, leaveEvent)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->leaveEvent(new QEvent(QEvent::Type::MouseMove));
    ASSERT_FALSE(m_memoryItemWidget->m_ishover);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, mousePressEvent)
{
    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::ColorType::LightType);
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                        m_memoryItemWidget->pos(), Qt::MouseButton::RightButton,
                                                        Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_memoryItemWidget->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                        m_memoryItemWidget->pos(), Qt::MouseButton::LeftButton,
                                                        Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_TRUE(m_memoryItemWidget->m_ispress);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, mouseReleaseEvent)
{
    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::ColorType::LightType);
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                          m_memoryItemWidget->pos(), Qt::MouseButton::RightButton,
                                                          Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_memoryItemWidget->mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                                          m_memoryItemWidget->pos(), Qt::MouseButton::LeftButton,
                                                          Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_FALSE(m_memoryItemWidget->m_ispress);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

//TEST_F(Ut_MemoryItemWidget, contextMenuEvent)
//{
//    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
//    m_memoryItemWidget->contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Reason::Mouse,
//                                                               QPoint(0, 0), QPoint(0, 0),
//                                                               Qt::KeyboardModifier::NoModifier));
//    ASSERT_FALSE(m_memoryItemWidget->m_ispress);
//    DSettings::deleteInstance();
//    MemoryPublic::deleteInstance();
//}

TEST_F(Ut_MemoryItemWidget, paintEvent)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    QPaintEvent *event = new QPaintEvent(m_memoryItemWidget->rect());
    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_memoryItemWidget->m_ishover = true;
    m_memoryItemWidget->m_ispress = true;
    m_memoryItemWidget->paintEvent(event);
    m_memoryItemWidget->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_memoryItemWidget->paintEvent(event);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, themetypechanged)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->themetypechanged(1);
    ASSERT_EQ(m_memoryItemWidget->m_label->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text).name(),
              "#303030");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, setNextItemHover)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->setNextItemHover();
    ASSERT_TRUE(m_memoryItemWidget->m_ishover);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}
