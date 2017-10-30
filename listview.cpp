#include "listview.h"
#include <QPainter>
#include <QDebug>

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    rowHeight = 40;
    padding = 10;
    offsetY = 0;
    scrollBarWidth = 6;
    scrollBarPadding = 8;
    isDragScrollBar = false;

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

void ListView::clearItems()
{
    items.clear();

    repaint();
}

void ListView::scrollToBottom()
{
    offsetY = getItemsTotalHeight() - rect().height();

    repaint();
}

void ListView::insertStrToItem(const QString &str)
{
    items.last()->insert(str);

    repaint();
}

void ListView::backspace()
{
    items.last()->backspace();

    repaint();
}

void ListView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw background and content.
    int drawHeight = 0;
    int count = 0;

    for (ListItem *item : items) {
        if (count >= offsetY / rowHeight) {
            item->drawBackground(QRect(0, count * rowHeight - offsetY, width(), rowHeight), &painter);
            item->drawContent(QRect(padding,
                                    count * rowHeight - offsetY,
                                    width() - padding * 2 - scrollBarPadding,
                                    rowHeight), &painter);

            drawHeight += rowHeight;

            if (drawHeight > rect().height())
                break;
        }

        ++count;
    }

    // Draw scrollbar.
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#000000"));

    if (isDragScrollBar) {
        painter.setOpacity(0.7);
    }else {
        painter.setOpacity(0.5);
    }

    if (items.count() > 4)
        painter.drawRoundedRect(QRect(width() - scrollBarPadding, getScrollBarY(), scrollBarWidth, getScrollBarHeight()), 5, 5);
}

void ListView::mouseMoveEvent(QMouseEvent *e)
{
    if (isDragScrollBar) {
        offsetY = adjustOffsetY(e->y() / (rect().height() * 1.0) * getItemsTotalHeight());

        repaint();
    }
}

void ListView::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (e->x() >= getScrollBarX()) {
            isDragScrollBar = true;

            offsetY = adjustOffsetY(e->y() / (rect().height() * 1.0) * getItemsTotalHeight());
        }

        repaint();
    }
}

void ListView::mouseReleaseEvent(QMouseEvent *e)
{
    if (isDragScrollBar)
        isDragScrollBar = false;

    repaint();
}

int ListView::getItemsTotalHeight() const
{
    return items.count() * rowHeight;
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
