#include "memorylistwidget.h"
#include <QMouseEvent>
#include <QColor>
#include <QPainter>

MemoryListWidget::MemoryListWidget(QWidget *parent)
    : QListWidget(parent)
{

}

void MemoryListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        mousepoint = event->pos();
        if (this->itemAt(mousepoint)) {
            clickrow = this->row(this->itemAt(mousepoint));
//            this->itemAt(mousepoint)->setBackground(QBrush(QColor(0, 0, 0)));
        }
        emit itempressed(clickrow);
    }

    QListWidget::mousePressEvent(event);
}

void MemoryListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit itemreleased();
    if (this->itemAt(event->pos()) && this->row(this->itemAt(event->pos())) == clickrow) {
        emit itemselected(clickrow);
    }
    QListWidget::mouseReleaseEvent(event);
}

MemoryListWidget::~MemoryListWidget()
{

}

MemoryListWidgetItem::MemoryListWidgetItem(MemoryListWidget *view)
    : QListWidgetItem(view)
{
    view->takeItem(view->count() - 1);
}

MemoryListWidgetItem::~MemoryListWidgetItem()
{

}
