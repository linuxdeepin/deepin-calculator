#include "ut_scihistorywidget.h"

#include "../../src/widgets/scihistorywidget.h"
#include "../../src/memorypublic.h"


Ut_SciHistoryWidget::Ut_SciHistoryWidget()
{

}

TEST_F(Ut_SciHistoryWidget, m_buttonbox0_clicked)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    QTest::mouseClick(m_scihistorywidget->m_buttonbox->button(0), Qt::LeftButton);
    ASSERT_EQ(m_scihistorywidget->m_stacklayout->currentIndex(), 0);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, m_buttonbox1_clicked)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    QTest::mouseClick(m_scihistorywidget->m_buttonbox->button(1), Qt::LeftButton);
    ASSERT_EQ(m_scihistorywidget->m_stacklayout->currentIndex(), 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, m_clearbtn_clicked)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    QTest::mouseClick(m_scihistorywidget->m_buttonbox->button(0), Qt::LeftButton);
    QTest::mouseClick(m_scihistorywidget->m_clearbutton, Qt::LeftButton);
    QTest::mouseClick(m_scihistorywidget->m_buttonbox->button(1), Qt::LeftButton);
    QTest::mouseClick(m_scihistorywidget->m_clearbutton, Qt::LeftButton);
    ASSERT_TRUE(m_scihistorywidget->m_clearbutton->isHidden());
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, m_clearbtn_space)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    QTest::mouseClick(m_scihistorywidget->m_buttonbox->button(0), Qt::LeftButton);
    QTest::keyPress(m_scihistorywidget->m_clearbutton, Qt::Key_Space);
    QTest::mouseClick(m_scihistorywidget->m_buttonbox->button(1), Qt::LeftButton);
    QTest::keyPress(m_scihistorywidget->m_clearbutton, Qt::Key_Space);
    ASSERT_TRUE(m_scihistorywidget->m_clearbutton->isHidden());
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, focusOnButtonbox)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    m_scihistorywidget->m_stacklayout->setCurrentIndex(0);
    m_scihistorywidget->focusOnButtonbox();
    m_scihistorywidget->m_stacklayout->setCurrentIndex(1);
    m_scihistorywidget->focusOnButtonbox();
    ASSERT_TRUE(m_scihistorywidget->m_buttonbox->button(1)->isCheckable());
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, memoryFunctions)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    m_scihistorywidget->memoryFunctions(SciHistoryWidget::memOperate::memoryplus, Quantity(0), 0);
    m_scihistorywidget->memoryFunctions(SciHistoryWidget::memOperate::memoryclean, Quantity(0), 0);
    m_scihistorywidget->memoryFunctions(SciHistoryWidget::memOperate::memoryminus, Quantity(0), 0);
    m_scihistorywidget->memoryFunctions(SciHistoryWidget::memOperate::widgetplus, Quantity(2), 0);
    m_scihistorywidget->memoryFunctions(SciHistoryWidget::memOperate::widgetminus, Quantity(1), 0);
    m_scihistorywidget->memoryFunctions(SciHistoryWidget::memOperate::generateData, Quantity(1), 0);
    ASSERT_EQ(m_scihistorywidget->m_memorywidget->findChild<MemoryListWidget *>()->count(), 2);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, mouseMoveEvent)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    m_scihistorywidget->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_scihistorywidget->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, keyPressEvent)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
    m_scihistorywidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    //无法setfocus,无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_SciHistoryWidget, themeChanged)
{
    SciHistoryWidget *m_scihistorywidget = new SciHistoryWidget;
//    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_scihistorywidget->themeChanged(0);
    m_scihistorywidget->themeChanged(1);
    ASSERT_EQ(m_scihistorywidget->m_clearbutton->m_currentUrl, ":/assets/images/light/empty_normal.svg");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

