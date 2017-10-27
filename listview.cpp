#include "listview.h"
#include <QPainter>
#include <QDebug>

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    rowHeight = 41;
    padding = 10;
    offsetY = 40;
    scrollBarWidth = 6;
    scrollBarPadding = 8;

    setMouseTracking(true);
}

ListView::~ListView()
{
}

void ListView::addItem(ListItem *item)
{
    items << item;

    repaint();
}

int ListView::getItemsTotalHeight() const
{
    return items.count() * rowHeight;
}

int ListView::getScrollBarY() const
{
    return offsetY / (getItemsTotalHeight() * 1.0) * rect().height();
}

int ListView::getScrollBarHeight() const
{
    return rect().height() * 1.0 / getItemsTotalHeight() * rect().height();
}

void ListView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#E7E7E7"));
    painter.drawRect(rect());

    painter.setPen("#333333");

    // Draw background and content.
    int drawHeight = 0;
    int count = 0;
    
    for (ListItem *item : items) {
        if (count >= offsetY / rowHeight) {
            item->drawBackground(QRect(0, count * rowHeight - offsetY, width(), rowHeight), &painter);
            item->drawContent(QRect(padding + scrollBarPadding,
                                    count * rowHeight - offsetY,
                                    width() - padding * 2 - scrollBarPadding,
                                    rowHeight), &painter);
        }

        drawHeight += rowHeight;

        if (drawHeight > rect().height())
            break;

        ++count;
    }

    // Draw scrollbar.
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#000000"));
    painter.setOpacity(0.5);
    painter.drawRoundedRect(QRect(width() - scrollBarPadding, getScrollBarY(), scrollBarWidth, getScrollBarHeight() - scrollBarPadding), 5, 5);
}
