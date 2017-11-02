#include "listview.h"
#include <QPainter>
#include <QDebug>
#include <QWheelEvent>

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    rowHeight = 44;
    padding = 10;
    offsetY = 0;
    scrollBarWidth = 6;
    scrollBarPadding = 8;
    isDragScrollBar = false;
    isShowScrollBar = false;

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
    listItems.clear();

    update();
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

void ListView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#FBFBFB"));
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
                                    rect().width() - padding * 2 - scrollBarPadding,
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

    if (isDragScrollBar) {
        painter.setOpacity(0.7);
        isShowScrollBar = true;
    }else {
        painter.setOpacity(0.5);
    }

    if (listItems.count() > 4)
        if (isShowScrollBar)
            painter.drawRoundedRect(QRect(width() - scrollBarPadding, getScrollBarY(), scrollBarWidth, getScrollBarHeight()), 5, 5);
}

void ListView::mouseMoveEvent(QMouseEvent *e)
{
    if (e->x() > getScrollBarX()) {
        isShowScrollBar = true;
        update();
    } else {
        isShowScrollBar = false;
        update();
    }

    if (isDragScrollBar) {
        offsetY = adjustOffsetY(e->y() / (rect().height() * 1.0) * getItemsTotalHeight());

        update();
    }
}

void ListView::mousePressEvent(QMouseEvent *e)
{
    if (listItems.count() > 4) {
        if (e->button() == Qt::LeftButton) {
            if (e->x() > getScrollBarX()) {
                isDragScrollBar = true;

                offsetY = adjustOffsetY(e->y() / (rect().height() * 1.0) * getItemsTotalHeight());
            }

            update();
        }
    }
}

void ListView::mouseReleaseEvent(QMouseEvent *e)
{
    if (isDragScrollBar)
        isDragScrollBar = false;

    update();
}

void ListView::wheelEvent(QWheelEvent *e)
{
    if (listItems.count() < 4)
        return;

    if (e->delta() == -120) {
        offsetY = adjustOffsetY(offsetY + rowHeight);
    } else {
        offsetY = adjustOffsetY(offsetY - rowHeight);
    }

    isShowScrollBar = true;

    update();
}

void ListView::leaveEvent(QEvent *)
{
    isShowScrollBar = false;

    update();
}

int ListView::getItemsTotalHeight() const
{
    return listItems.count() * rowHeight;
}

int ListView::getScrollBarHeight() const
{
    return rect().height() * 1.0 / getItemsTotalHeight() * rect().height();
}

int ListView::getScrollBarX() const
{
    return rect().width() - scrollBarWidth - scrollBarPadding;
}

int ListView::getScrollBarY() const
{
    return offsetY / (getItemsTotalHeight() * 1.0) * rect().height();
}

int ListView::adjustOffsetY(const int &offset) const
{
    return qMax(0, qMin(offset, getItemsTotalHeight() - rect().height()));
}
