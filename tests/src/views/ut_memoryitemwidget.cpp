#include "ut_memoryitemwidget.h"
#include "../../src/views/memoryitemwidget.h"
#include "../../src/dsettings.h"
#include "../../src/memorypublic.h"

Ut_MemoryItemWidget::Ut_MemoryItemWidget()
{

}

TEST_F(Ut_MemoryItemWidget, connects)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->m_btnplus->clicked();
    m_memoryItemWidget->m_btnminus->clicked();
    m_memoryItemWidget->m_btnclean->clicked();
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, enterEvent)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    QEvent *e = new QEvent(QEvent::Type::MouseMove);
    m_memoryItemWidget->enterEvent(e);
    ASSERT_TRUE(m_memoryItemWidget->m_ishover);
    delete e;
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, leaveEvent)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    QEvent *e = new QEvent(QEvent::Type::MouseMove);
    m_memoryItemWidget->leaveEvent(e);
    ASSERT_FALSE(m_memoryItemWidget->m_ishover);
    delete e;
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, mousePressEvent)
{
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    QMouseEvent *m = new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                     m_memoryItemWidget->pos(), Qt::MouseButton::RightButton,
                                     Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
    m_memoryItemWidget->mousePressEvent(m);
    QMouseEvent *m1 = new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                      m_memoryItemWidget->pos(), Qt::MouseButton::LeftButton,
                                      Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
    m_memoryItemWidget->mousePressEvent(m1);
    ASSERT_TRUE(m_memoryItemWidget->m_ispress);
    delete m;
    delete m1;
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, mouseReleaseEvent)
{
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    QMouseEvent *m = new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                     m_memoryItemWidget->pos(), Qt::MouseButton::LeftButton,
                                     Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
    m_memoryItemWidget->mouseReleaseEvent(m);
    ASSERT_FALSE(m_memoryItemWidget->m_ispress);
    delete m;
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

QAction *stub_exec(const QPoint &pos, QAction *at = nullptr)
{
    Q_UNUSED(pos)
    Q_UNUSED(at)

    return nullptr;
}

TEST_F(Ut_MemoryItemWidget, contextMenuEvent)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    Stub stub;
    stub.set((QAction * (QMenu::*)(const QPoint &, QAction *))ADDR(QMenu, exec), stub_exec);
    m_memoryItemWidget->m_isExpressionEmpty = true;
    QContextMenuEvent *e = new QContextMenuEvent(QContextMenuEvent::Reason::Mouse,
                                                 m_memoryItemWidget->pos(), m_memoryItemWidget->pos(),
                                                 Qt::KeyboardModifier::NoModifier);
    m_memoryItemWidget->contextMenuEvent(e);
//    stub.set((QAction * (QMenu::*)(const QPoint &, QAction *))ADDR(QMenu, exec), stub_exec);
//    m_memoryItemWidget->m_isExpressionEmpty = false;
//    m_memoryItemWidget->contextMenuEvent(e);

    ASSERT_FALSE(m_memoryItemWidget->m_ispress);
    delete e;
}

TEST_F(Ut_MemoryItemWidget, showTextEditMenuByAltM)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    Stub stub;
    stub.set((QAction * (QMenu::*)(const QPoint &, QAction *))ADDR(QMenu, exec), stub_exec);
//    QContextMenuEvent *e = new QContextMenuEvent(QContextMenuEvent::Reason::Keyboard, m_memoryItemWidget->pos());
    m_memoryItemWidget->m_isExpressionEmpty = true;
    m_memoryItemWidget->showTextEditMenuByAltM();

//    stub.set((QAction * (QMenu::*)(const QPoint &, QAction *))ADDR(QMenu, exec), stub_exec);
//    m_memoryItemWidget->m_isExpressionEmpty = false;
//    m_memoryItemWidget->showTextEditMenuByAltM();

    ASSERT_FALSE(m_memoryItemWidget->m_ispress);
}

TEST_F(Ut_MemoryItemWidget, paintEvent)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    QPaintEvent *event = new QPaintEvent(m_memoryItemWidget->rect());
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_memoryItemWidget->m_ishover = true;
    m_memoryItemWidget->m_ispress = true;
    m_memoryItemWidget->paintEvent(event);
    m_memoryItemWidget->update();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_memoryItemWidget->paintEvent(event);
    delete event;
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, themetypechanged)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->themetypechanged(1);
    ASSERT_EQ(m_memoryItemWidget->m_label->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text).name(),
              "#303030");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryItemWidget, setNextItemHover)
{
    MemoryItemWidget *m_memoryItemWidget = new MemoryItemWidget;
    m_memoryItemWidget->setNextItemHover();
    ASSERT_TRUE(m_memoryItemWidget->m_ishover);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}
