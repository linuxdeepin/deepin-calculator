/*
 * 1. @类名:    MemoryListWidget
 * 2. @作者:    京洲 ut000490
 * 3. @日期:    2020-07-01
 * 4. @说明:    内存ListWidget
 */
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
        m_mousepoint = event->pos();
        if (this->itemAt(m_mousepoint)) {
            m_clickrow = this->row(this->itemAt(m_mousepoint));
//            this->itemAt(mousepoint)->setBackground(QBrush(QColor(0, 0, 0)));
        }
        emit itempressed(m_clickrow);
    }

    QListWidget::mousePressEvent(event);
}

void MemoryListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->itemAt(event->pos()) && this->row(this->itemAt(event->pos())) == m_clickrow) {
        emit itemselected(m_clickrow);
    }
    m_clickrow = -1;
    QListWidget::mouseReleaseEvent(event);
}

MemoryListWidget::~MemoryListWidget()
{

}
