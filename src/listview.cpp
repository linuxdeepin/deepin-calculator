#include "listview.h"
#include <QPainter>
#include <QDebug>
#include <QWheelEvent>

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    rowHeight = 44;
    padding = 8;
    offsetY = 0;
    scrollbarWidth = 6;
    scrollbarPadding = 8;
    isDragScrollbar = false;
    isShowScrollbar = false;

    setMouseTracking(true);
}

ListView::~ListView()
{
}

void ListView::addItem(ListItem *item)
{
    listItems << item;
}

void ListView::clearAllItems()
{
    if (!listItems.isEmpty()) {
        listItems.clear();
        update();
    }
}

void ListView::clearLastItem()
{
    if (!listItems.isEmpty()) {
        listItems.removeLast();
        update();
    }
}

void ListView::scrollToBottom()
{
    offsetY = getItemsTotalHeight() - rect().height();

    update();
}

ListItem* ListView::lastItem()
{
    return listItems.last();
}

void ListView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#FFFFFF"));
    painter.drawRect(rect());

    // Draw content.
    int drawHeight = 0;
    int count = 0;

    for (ListItem *item : listItems) {
        if (count >= offsetY / rowHeight) {
            const int lastIndex = listItems.indexOf(listItems.last());
            const bool isLast = lastIndex == count;

            item->drawBackground(QRect(0, count * rowHeight - offsetY, rect().width(), rowHeight), &painter);
            item->drawContent(QRect(padding,
                                    count * rowHeight - offsetY,
                                    rect().width() - padding * 2 - scrollbarPadding,
                                    rowHeight), &painter, isLast);

            drawHeight += rowHeight;

            if (drawHeight > rect().height() + rowHeight)
                break;
        }

        ++count;
    }

    // Draw scrollbar.
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#000000"));
    drawScrollbar(&painter);
}

void ListView::drawScrollbar(QPainter *painter)
{
    if (getItemsTotalHeight() > rect().height() + rowHeight) {
        if (!isShowScrollbar) {
            return;
        }

        if (isDragScrollbar) {
            painter->setOpacity(0.7);
        } else {
            painter->setOpacity(0.5);
        }

        const int barRadius = 4;
        painter->drawRoundedRect(QRect(rect().width() - scrollbarPadding,
                                       getScrollbarY(),
                                       scrollbarWidth,
                                       getScrollbarHeight()), barRadius, barRadius);
    }
}

void ListView::mouseMoveEvent(QMouseEvent *e)
{
    if (getItemsTotalHeight() > rect().height() + rowHeight) {
        if (isDragScrollbar) {
            offsetY = adjustOffsetY((e->y() - getScrollbarHeight() / 2) / (rect().height() * 1.0) * getItemsTotalHeight());
            isShowScrollbar = true;
            update();
        } else if (e->x() > getScrollbarX()) {
            isShowScrollbar = true;
            update();
        } else {
            isShowScrollbar = false;
            update();
        }
    }
}

void ListView::mousePressEvent(QMouseEvent *e)
{
    if (getItemsTotalHeight() > rect().height() + rowHeight) {
        if (e->button() == Qt::LeftButton) {
            if (e->x() > getScrollbarX()) {
                isDragScrollbar = true;
                offsetY = adjustOffsetY((e->y() - getScrollbarHeight() / 2) / (rect().height() * 1.0) * getItemsTotalHeight());
                update();
            }
        }
    }
}

void ListView::mouseReleaseEvent(QMouseEvent *e)
{
    if (isDragScrollbar) {
        isDragScrollbar = false;
    }

    update();
}

void ListView::wheelEvent(QWheelEvent *e)
{
    if (getItemsTotalHeight() < rect().height() + rowHeight) {
        return;
    }

    if (e->delta() == -120) {
        offsetY = adjustOffsetY(offsetY + rowHeight);
    } else {
        offsetY = adjustOffsetY(offsetY - rowHeight);
    }

    isShowScrollbar = true;

    update();
}

void ListView::leaveEvent(QEvent *)
{
    isShowScrollbar = false;

    update();
}

int ListView::getItemsTotalHeight() const
{
    return listItems.count() * rowHeight;
}

int ListView::getScrollbarHeight() const
{
    return static_cast<int>(rect().height() * 1.0 / getItemsTotalHeight() * rect().height());
}

int ListView::getScrollbarX() const
{
    return rect().width() - scrollbarWidth - scrollbarPadding;
}

int ListView::getScrollbarY() const
{
    return static_cast<int>(offsetY / (getItemsTotalHeight() * 1.0) * rect().height());
}

int ListView::adjustOffsetY(const int &offset) const
{
    return qMax(0, qMin(offset, getItemsTotalHeight() - rect().height()));
}
