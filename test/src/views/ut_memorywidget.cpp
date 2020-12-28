#include "ut_memorywidget.h"
#include "../../src/views/memorywidget.h"
#include "../../src/memorypublic.h"

Ut_MemoryWidget::Ut_MemoryWidget()
{

}

TEST_F(Ut_MemoryWidget, space)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    QListWidgetItem *item1 = new QListWidgetItem;
    item1->setFlags(Qt::ItemIsEditable);
    MemoryItemWidget *widget = new MemoryItemWidget;
    m_memoryWidget->m_listwidget->insertItem(0, item1);
    m_memoryWidget->m_listwidget->setItemWidget(item1, widget);
    widget->setTextLabel("1");
    m_memoryWidget->m_list.insert(0, Quantity(1));
    m_memoryWidget->m_isempty = false;
    m_memoryWidget->m_listwidget->setCurrentRow(0);
    m_memoryWidget->m_listwidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    m_memoryWidget->m_clearbutton->clicked();
    m_memoryWidget->m_listwidget->itemselected(0, false);
    m_memoryWidget->m_listwidget->focus(0);
    m_memoryWidget->m_listwidget->focus(1);
    //无ASSERT
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, focus)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->generateData(Quantity(2));
    m_memoryWidget->generateData(Quantity(3));
    m_memoryWidget->setFocus();
    m_memoryWidget->m_listwidget->setCurrentRow(1);
    m_memoryWidget->m_listwidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    m_memoryWidget->m_listwidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
    m_memoryWidget->m_listwidget->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    ASSERT_EQ(m_memoryWidget->m_listwidget->currentRow(), 1);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, generateData)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    QTest::mouseClick(static_cast<MemoryItemWidget *>(m_memoryWidget->m_listwidget->itemWidget(m_memoryWidget->m_listwidget->item(0)))->m_btnclean, Qt::LeftButton);
    for (int i = 0; i < 500; i++)
        m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->m_listwidget->setCurrentRow(1);
    QTest::mouseClick(static_cast<MemoryItemWidget *>(m_memoryWidget->m_listwidget->itemWidget(m_memoryWidget->m_listwidget->item(0)))->m_btnplus, Qt::LeftButton);
    QTest::mouseClick(static_cast<MemoryItemWidget *>(m_memoryWidget->m_listwidget->itemWidget(m_memoryWidget->m_listwidget->item(0)))->m_btnminus, Qt::LeftButton);
    ASSERT_EQ(m_memoryWidget->m_listwidget->count(), 500);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, mousePressEvent)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->mousePressEvent(new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                                    m_memoryWidget->pos(), Qt::MouseButton::LeftButton,
                                                    Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier));
    ASSERT_EQ(m_memoryWidget->m_mousepoint, m_memoryWidget->pos());
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, eventFilter)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->m_calculatormode = 0;
    m_memoryWidget->eventFilter(m_memoryWidget->m_listwidget, static_cast <QEvent *>(new QKeyEvent(QKeyEvent::Type::KeyPress, Qt::Key_Tab, Qt::KeyboardModifier::NoModifier)));
//    ASSERT_TRUE(m_memoryWidget->m_listwidget->hasFocus());
    //无ASSERT,此处无法setfocus
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, focusInEvent)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->m_calculatormode = 1;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->focusInEvent(new QFocusEvent(QFocusEvent::Type::FocusIn));
    //无ASSERT
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, memoryplus)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->memoryplus(Quantity(1));
    ASSERT_EQ(static_cast<MemoryItemWidget *>(m_memoryWidget->m_listwidget->itemWidget(m_memoryWidget->m_listwidget->item(0)))->textLabel(), "2");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, memoryminus)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->memoryminus(Quantity(1));
    ASSERT_EQ(static_cast<MemoryItemWidget *>(m_memoryWidget->m_listwidget->itemWidget(m_memoryWidget->m_listwidget->item(0)))->textLabel(), "0");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, memoryclean)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->memoryclean();
    ASSERT_TRUE(m_memoryWidget->m_isempty);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, emptymemoryfontcolor)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->m_themetype = 1;
    m_memoryWidget->emptymemoryfontcolor();
    QColor color(85, 85, 85);
    color.setAlphaF(0.4);
    ASSERT_EQ(m_memoryWidget->m_label->palette().color(DPalette::WindowText), color);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, getfirstnumber)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    ASSERT_EQ(m_memoryWidget->getfirstnumber().first, "1");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, widgetplusslot)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->widgetplusslot(0, Quantity(1));
    ASSERT_EQ(static_cast<MemoryItemWidget *>(m_memoryWidget->m_listwidget->itemWidget(m_memoryWidget->m_listwidget->item(0)))->textLabel(), "2");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, widgetminusslot)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->widgetminusslot(0, Quantity(1));
    ASSERT_EQ(static_cast<MemoryItemWidget *>(m_memoryWidget->m_listwidget->itemWidget(m_memoryWidget->m_listwidget->item(0)))->textLabel(), "0");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, isWidgetEmpty)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->m_isempty = true;
    ASSERT_TRUE(m_memoryWidget->isWidgetEmpty());
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, getMemoryWidget)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    ASSERT_EQ(static_cast<MemoryItemWidget *>(m_memoryWidget->getMemoryWidget()->itemWidget(m_memoryWidget->m_listwidget->item(0)))->textLabel(), "1");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, programmerResult)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    HNumber number("1.5");
    ASSERT_EQ(m_memoryWidget->programmerResult(Quantity(number), true), "1");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, resetAllLabelByBase)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    HNumber number("1.5");
    m_memoryWidget->generateData(Quantity(number));
    m_memoryWidget->m_calculatormode = 2;
    m_memoryWidget->resetAllLabelByBase();
    ASSERT_EQ(static_cast<MemoryItemWidget *>(m_memoryWidget->getMemoryWidget()->itemWidget(m_memoryWidget->m_listwidget->item(0)))->textLabel(), "1");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, expressionempty)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->m_isempty = false;
    m_memoryWidget->expressionempty(false);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, widgetcleanslot)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->m_calculatormode = 1;
    m_memoryWidget->widgetcleanslot(0, 0, false);
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->m_calculatormode = 0;
    m_memoryWidget->widgetcleanslot(0, 0, false);
    ASSERT_EQ(m_memoryWidget->m_listwidget->count(), 2);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, formatExpression)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    ASSERT_EQ(m_memoryWidget->formatExpression("123＋2321"), "123+2321");
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, setitemwordwrap)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->generateData(Quantity(1));
    m_memoryWidget->m_calculatormode = 2;
    m_memoryWidget->setitemwordwrap("12345678901234", 0);
    m_memoryWidget->setitemwordwrap("1234567890123456789012", 0);
    m_memoryWidget->m_calculatormode = 0;
    m_memoryWidget->setitemwordwrap("12345678901234E", 0);
    m_memoryWidget->setitemwordwrap("1234567890123456789012", 0);
    m_memoryWidget->m_calculatormode = 1;
    m_memoryWidget->setitemwordwrap("123456789012345678901E", 0);
    m_memoryWidget->setitemwordwrap("1234567890123456789012", 0);
    ASSERT_EQ(m_memoryWidget->m_line, 2);
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryWidget, setThemeType)
{
    MemoryWidget *m_memoryWidget = new MemoryWidget;
    m_memoryWidget->setThemeType(2);
    m_memoryWidget->setThemeType(1);
    ASSERT_EQ(m_memoryWidget->m_themetype, 1);
    MemoryPublic::deleteInstance();
}
