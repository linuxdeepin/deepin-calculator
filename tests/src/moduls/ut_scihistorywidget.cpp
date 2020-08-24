#include "ut_scihistorywidget.h"
#define private public
#include "src/modules/scihistorywidget.h"
#undef private

Ut_SciHistoryWidget::Ut_SciHistoryWidget()
{

}

TEST_F(Ut_SciHistoryWidget, m_buttonbox_clicked)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
//    QApplication::focusWidget()->clearFocus();
    QTest::mouseClick(m_scihistorywidget->m_buttonbox->button(0), Qt::LeftButton);
    ASSERT_EQ(m_scihistorywidget->m_stacklayout->currentIndex(), 0);
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}
