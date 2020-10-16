#include "programmerarrowdelegate.h"

#include <DStyleHelper>
#include <DApplicationHelper>
#include "memorylistwidget.h"

ProgrammerArrowDelegate::ProgrammerArrowDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ProgrammerArrowDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //边框绘制
    MemoryListWidget *dragWidget = qobject_cast<MemoryListWidget *>(option.styleObject);
//    QRectF rect = dragWidget->rect(); //获取memorylistwidget.rect绘制背景色
    painter->setRenderHint(QPainter::Antialiasing, true);
    if (dragWidget->hasFocus()) {
//        qDebug() << "paint";
        qDebug() << dragWidget->row(dragWidget->currentItem());
        QRectF itemrect(dragWidget->visualItemRect(dragWidget->currentItem()));
        QRectF frame(itemrect.left() + 1, itemrect.top() + 1, itemrect.width() - 2, itemrect.height() - 2);
        QPen pen;
        pen.setColor(Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color().name());
        pen.setWidth(4);
        painter->setPen(pen);
        painter->setBrush(Qt::red);
        painter->drawRoundedRect(frame, 4, 4); //focus边框
    }
    if (option.state & QStyle::State_MouseOver) {
        qDebug() << "123";
        qDebug() << option.index.row();
        painter->setPen(Qt::blue);
        painter->setBrush(Qt::red);
        QRect rect = option.rect;
        painter->drawRoundedRect(rect, 4, 4);
    }
    QStyledItemDelegate::paint(painter, option, index);
}

void ProgrammerArrowDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
