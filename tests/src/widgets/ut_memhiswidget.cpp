#include "ut_memhiswidget.h"

#include "../../src/widgets/memhiswidget.h"
#include "../../src/memorypublic.h"
#include <DButtonBox>


Ut_MemHisWidget::Ut_MemHisWidget()
{

}

TEST_F(Ut_MemHisWidget, space)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->m_historyBtn->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    m_memhiswidget->m_memoryBtn->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    m_memhiswidget->m_clearButton->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    //无ASSERT
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, filledMem)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    MemoryPublic *m_mempublic = MemoryPublic::instance();
//    m_memhiswidget->m_memoryWidget->generateData(Quantity(1),false);
    m_mempublic->generateData(Quantity(1));
    ASSERT_EQ(m_memhiswidget->m_isshowM, true);
    //无ASSERT
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, emptyMem)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    MemoryPublic *m_mempublic = MemoryPublic::instance();
//    m_memhiswidget->m_memoryWidget->memoryclean();
    m_mempublic->memoryclean();
    ASSERT_EQ(m_memhiswidget->m_isshowM, false);
    //无ASSERT
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, hisbtnhidden)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->m_listModel->hisbtnhidden();
    ASSERT_EQ(m_memhiswidget->m_isshowH, false);
    //无ASSERT
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, clicked)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->m_historyBtn->clicked();
    m_memhiswidget->m_memoryBtn->clicked();
    m_memhiswidget->m_clearButton->clicked();
    //无ASSERT
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, focusOnButtonbox)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->focusOnButtonbox(Qt::TabFocusReason);
    m_memhiswidget->m_stackWidget->setCurrentIndex(0);
    ASSERT_TRUE(m_memhiswidget->m_buttonBox->button(0)->isChecked());
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, resetFocus)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->m_isshowH = false;
    m_memhiswidget->m_isshowM = true;
    m_memhiswidget->resetFocus();
    ASSERT_EQ(m_memhiswidget->m_memoryWidget->focusPolicy(), Qt::TabFocus);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, keyPressEvent)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->m_historyBtn->setFocus();
    m_memhiswidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_memhiswidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    m_memhiswidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier));
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, eventFilter)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->eventFilter(m_memhiswidget->m_historyBtn, static_cast <QEvent *>(new QFocusEvent(QEvent::Type::FocusIn, Qt::TabFocusReason)));
    m_memhiswidget->m_clearButton->setHidden(false);
    m_memhiswidget->eventFilter(m_memhiswidget, static_cast <QEvent *>(new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier)));
    m_memhiswidget->eventFilter(m_memhiswidget, static_cast <QEvent *>(new QFocusEvent(QEvent::Type::FocusOut, Qt::MouseFocusReason)));
    ASSERT_FALSE(m_memhiswidget->hasFocus());
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, focusInEvent)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->m_stackWidget->setCurrentIndex(0);
    m_memhiswidget->focusInEvent(new QFocusEvent(QEvent::Type::FocusIn, Qt::TabFocusReason));
    m_memhiswidget->m_stackWidget->setCurrentIndex(1);
    m_memhiswidget->focusInEvent(new QFocusEvent(QEvent::Type::FocusIn, Qt::TabFocusReason));
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, themeChanged)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->themeChanged(1);
    ASSERT_EQ(m_memhiswidget->m_themeType, 1);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, iconChanged)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->iconChanged(1, 0);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemHisWidget, historyfilled)
{
    MemHisWidget *m_memhiswidget = new MemHisWidget;
    m_memhiswidget->historyfilled();
    bool b1 = m_memhiswidget->m_listView->focusPolicy() == Qt::TabFocus;
    bool b2 = m_memhiswidget->m_isshowH;
    ASSERT_TRUE(b1 && b2);
    MemoryPublic::deleteInstance();
}
