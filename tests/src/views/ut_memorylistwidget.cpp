#include "ut_memorylistwidget.h"
#include "../../src/views/memorylistwidget.h"
#include "../../src/dsettings.h"
#include "../../src/memorypublic.h"

Ut_MemoryListWidget::Ut_MemoryListWidget()
{

}

TEST_F(Ut_MemoryListWidget, mousePressEvent)
{
    MemoryListWidget *m_memoryListWidget = new MemoryListWidget;
    QMouseEvent *m = new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                     m_memoryListWidget->pos(), Qt::MouseButton::LeftButton,
                                     Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier);
    m_memoryListWidget->mousePressEvent(m);
//    qDebug() << m_memoryListWidget->m_clickrow;
    delete m;
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryListWidget, keyPressEvent)
{
    MemoryListWidget *m_memoryListWidget = new MemoryListWidget;
    QKeyEvent *k = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    QKeyEvent *k1 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    QKeyEvent *k2 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    QKeyEvent *k3 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    QKeyEvent *k4 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
    QKeyEvent *k5 = new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier);
    m_memoryListWidget->keyPressEvent(k);
    m_memoryListWidget->keyPressEvent(k1);
    m_memoryListWidget->keyPressEvent(k2);
    m_memoryListWidget->keyPressEvent(k3);
    m_memoryListWidget->keyPressEvent(k4);
    m_memoryListWidget->keyPressEvent(k5);
    delete k;
    delete k1;
    delete k2;
    delete k3;
    delete k4;
    delete k5;
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryListWidget, focusInEvent)
{
    MemoryListWidget *m_memoryListWidget = new MemoryListWidget;
    QFocusEvent *f = new QFocusEvent(QEvent::Type::FocusIn, Qt::OtherFocusReason);
    m_memoryListWidget->focusInEvent(f);
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}
