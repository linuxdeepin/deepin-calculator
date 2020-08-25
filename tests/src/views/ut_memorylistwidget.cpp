#include "ut_memorylistwidget.h"
#define private public
#include "src/views/memorylistwidget.h"
#undef private
#include "src/dsettings.h"
#include "src/memorypublic.h"

Ut_MemoryListWidget::Ut_MemoryListWidget()
{

}

TEST_F(Ut_MemoryListWidget, mousePressEvent)
{
    MemoryListWidget *m_memoryListWidget = new MemoryListWidget;
    m_memoryListWidget->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                        m_memoryListWidget->pos(), Qt::MouseButton::LeftButton,
                                                        Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier));
//    qDebug() << m_memoryListWidget->m_clickrow;
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryListWidget, keyPressEvent)
{
    MemoryListWidget *m_memoryListWidget = new MemoryListWidget;
    m_memoryListWidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    m_memoryListWidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
    m_memoryListWidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_memoryListWidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_memoryListWidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    m_memoryListWidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier));
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryListWidget, focusInEvent)
{
    MemoryListWidget *m_memoryListWidget = new MemoryListWidget;
    m_memoryListWidget->focusInEvent(new QFocusEvent(QEvent::Type::FocusIn, Qt::OtherFocusReason));
    //无ASSERT
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}
