#include "ut_memoryitemdelegate.h"
#define protected public
#include "src/views/memoryitemdelegate.h"
#undef protected

#include "src/dsettings.h"
#include "src/memorypublic.h"

Ut_MemoryItemDelegate::Ut_MemoryItemDelegate()
{

}

void Ut_MemoryItemDelegate::SetUp()
{

}

void Ut_MemoryItemDelegate::TearDown()
{

}

//TEST_F(Ut_MemoryItemDelegate, paint)
//{
//    MemoryItemDelegate *m_memoryItemDelegate = new MemoryItemDelegate;
//    MemoryListWidget *listwidget = new MemoryListWidget;
//    listwidget->setItemDelegate(m_memoryItemDelegate);
//    QPainter *painter = new QPainter();
//    QStyleOptionViewItem option;
//    QModelIndex index;
//    m_memoryItemDelegate->paint(painter, option, index);
////    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::ColorType::DarkType);
////    listwidget->repaint();
////    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::ColorType::LightType);
////    listwidget->update();
////    qDebug() << listwidget->palette().color(QPalette::ColorGroup::Current, QPalette::ColorRole::Window).name();
////    QPalette pal = m_memorywidget->palette();
////    QBrush brush = pal.background();
////    m_memorywidget->update();
////    m_memoryItemDelegate->paint()
////    qDebug() << m_memorywidget->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Base);
////    ASSERT_EQ(brush.color().name(), "#252525");
//    DSettings::deleteInstance();
//    MemoryPublic::deleteInstance();
//}
