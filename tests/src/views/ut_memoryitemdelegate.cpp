#include "ut_memoryitemdelegate.h"
#include "../../src/views/memoryitemdelegate.h"

#include "../../src/views/memorylistwidget.h"
#include <QPainter>

Ut_MemoryItemDelegate::Ut_MemoryItemDelegate()
{

}

//bool stub_focus_memdelegateT()
//{
//    return true;
//}

bool stub_focus_memdelegateF()
{
    return false;
}

TEST_F(Ut_MemoryItemDelegate, paint)
{
    MemoryItemDelegate *m_memoryItemDelegate = new MemoryItemDelegate;
    MemoryListWidget *listwidget = new MemoryListWidget;
    listwidget->setItemDelegate(m_memoryItemDelegate);

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(20, 20));
    MemoryItemWidget *widget = new MemoryItemWidget();
    widget->setFixedSize(QSize(20, 20));

    listwidget->insertItem(0, item);
    listwidget->setItemWidget(item, widget);

    QPainter *painter = new QPainter();
    QStyleOptionViewItem option;
    option.styleObject = listwidget;
    QModelIndex index;
    Stub stub;
    stub.set(ADDR(MemoryListWidget, hasFocus), stub_focus_memdelegateF);
//    Stub stub1;
//    stub1.set(ADDR(MemoryListWidget, rect), stub_rect_memdelegate);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_memoryItemDelegate->paint(painter, option, index);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_memoryItemDelegate->paint(painter, option, index);
//    listwidget->update();
//    qDebug() << listwidget->palette().color(QPalette::ColorGroup::Current, QPalette::ColorRole::Window).name();
//    QPalette pal = m_memorywidget->palette();
//    QBrush brush = pal.background();
//    m_memorywidget->update();
//    m_memoryItemDelegate->paint()
//    qDebug() << m_memorywidget->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Base);
//    ASSERT_EQ(brush.color().name(), "#252525");
}

TEST_F(Ut_MemoryItemDelegate, updateEditorGeometry)
{
    MemoryItemDelegate *m_memoryItemDelegate = new MemoryItemDelegate;
    m_memoryItemDelegate->updateEditorGeometry(new QWidget(), QStyleOptionViewItem(), QModelIndex());
}
