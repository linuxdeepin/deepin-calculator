#include "listview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    rowHeight = 35;
    offset = 0;
    isPress = false;
    isShowScrollbar = false;

    setMouseTracking(true);
    setFixedHeight(105);
    initTheme();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, [=] {
        initTheme();
    });
}

ListView::~ListView()
{
}

void ListView::initTheme()
{
    if (DThemeManager::instance()->theme() == "light") {
        backgroundColor = "#FBFBFB";
        fontColor = "#636363";
        scrollbarColor = "#000000";
    } else {
        backgroundColor = "#111111";
        fontColor = "#C3C3C3";
        scrollbarColor = "#FFFFFF";
    }
}

void ListView::addItem(const QString &text)
{
    listItems << text;

    scrollToBottom();
}

void ListView::clearItems()
{
    listItems.clear();

    scrollToBottom();
}

void ListView::scrollToBottom()
{
    offset = getItemsTotalHeight() - rect().height();

    update();
}

void ListView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(backgroundColor));
    painter.drawRect(rect());

    // Draw Items.
    painter.setPen(QColor(fontColor));

    int drawHeight = 0;
    int count = 0;
    for (QString item : listItems) {
        if (count >= offset / rowHeight) {
            QRect itemRect(0, count * rowHeight - offset, rect().width() - 15, rowHeight);
            painter.drawText(itemRect, Qt::AlignVCenter | Qt::AlignRight, item);

            drawHeight += rowHeight;

            if (drawHeight > rect().height())
                break;
        }

        ++count;
    }

    // Draw scrollbar
    if (!isShowScrollbar)
        return;

    painter.setBrush(QColor(scrollbarColor));

    if (isPress) {
        painter.setOpacity(0.7);
    } else {
        painter.setOpacity(0.5);
    }

    if (getItemsTotalHeight() > rect().height()) {
        const int radius = 4;
        painter.drawRoundedRect(QRect(rect().width() - 9,
                                      getScrollbarY(),
                                      6,
                                      getScrollbarHeight()), radius, radius);
    }
}

void ListView::mouseMoveEvent(QMouseEvent *e)
{
    if (getItemsTotalHeight() > rect().height()) {
        if (isPress) {
            offset = adjustOffset((e->y() - getScrollbarHeight() / 2) / (rect().height() * 1.0) * getItemsTotalHeight());
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
    if (getItemsTotalHeight() > rect().height()) {
        if (e->button() == Qt::LeftButton) {
            if (e->x() > getScrollbarX()) {
                isPress = true;
                offset = adjustOffset((e->y() - getScrollbarHeight() / 2) / (rect().height() * 1.0) * getItemsTotalHeight());
                update(); 
            }  
               
        }
    }
}

void ListView::mouseReleaseEvent(QMouseEvent *e)
{
    if (isPress) {
        isPress = false;
    }

    update();
}

void ListView::wheelEvent(QWheelEvent *e)
{
    if (getItemsTotalHeight() < rect().height()) {
        return;
    }

    if (e->delta() == -120) {
        offset = adjustOffset(offset + rowHeight);
    } else {
        offset = adjustOffset(offset - rowHeight);
    }

    isShowScrollbar = true;

    update();
}

void ListView::leaveEvent(QEvent *)
{
    isShowScrollbar = false;

    update();
}

int ListView::getItemsTotalHeight()
{
    return listItems.count() * rowHeight;
}

int ListView::getScrollbarHeight()
{
    return rect().height() * 1.0 / getItemsTotalHeight() * rect().height();
}

int ListView::getScrollbarX()
{
    return rect().width() - 10;
}

int ListView::getScrollbarY()
{
    return offset / (getItemsTotalHeight() * 1.0) * rect().height();
}

int ListView::adjustOffset(const int &offset)
{
    return qMax(0, qMin(offset, getItemsTotalHeight() - rect().height()));
}
