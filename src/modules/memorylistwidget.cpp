#include "memorylistwidget.h"
#include <QMouseEvent>
#include <QColor>
#include <QPainter>
#include <QDebug>
#include <DPalette>

DGUI_USE_NAMESPACE

MemoryListWidget::MemoryListWidget(QWidget *parent)
    : QListWidget(parent)
{
//    DPalette pal = this->palette();
//    pal.setColor(DPalette::Light, QColor(248, 248, 248));
//    this->setPalette(pal);
//    this->setAttribute(Qt::WA_StyledBackground, false);
//    DPalette pal = this->palette();
//    pal.setColor(DPalette::Light, QColor(248, 248, 248));
//    this->setPalette(pal);
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
    if (this->itemAt(event->pos()) && this->row(this->itemAt(event->pos())) == clickrow) {
        emit itemselected(clickrow);
    }
    clickrow = -1;
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
