#include "ut_memorylistwidget.h"
#define private public
#include "src/modules/memorylistwidget.h"
#undef private
#include "src/dsettings.h"
#include "src/modules/memorypublic.h"

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
    m_memoryListWidget->addItem("a");
    m_memoryListWidget->addItem("b");
    m_memoryListWidget->addItem("c");
    m_memoryListWidget->setCurrentRow(2);
    m_memoryListWidget->focusInEvent(new QFocusEvent(QEvent::Type::FocusIn, Qt::OtherFocusReason));
    ASSERT_EQ(m_memoryListWidget->currentRow(), 0);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}
