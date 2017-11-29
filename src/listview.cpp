#include "listview.h"
#include "dthememanager.h"
#include "utils.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

DWIDGET_USE_NAMESPACE

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    rowHeight = 35;
    padding = 10;
    offset = 0;
    scrollbarMinHeight = 30;
    isPress = false;
    isShowScrollbar = false;

    setMouseTracking(true);
    setMinimumHeight(105);
    setMaximumHeight(200);
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
        errorFontColor = "#F37D54";
        scrollbarColor = "#000000";
    } else {
        backgroundColor = "#111111";
        fontColor = "#C3C3C3";
        errorFontColor = "#F37D54";
        scrollbarColor = "#FFFFFF";
    }
}

void ListView::addItem(const QString &text)
{
    for (const QString &itemStr : listItems) {
        if (itemStr == text) {
            listItems.removeAt(listItems.indexOf(itemStr));
        }
    }

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

int ListView::getItemsCount()
{
    return listItems.count();
}

void ListView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw Items.
    painter.setPen(QColor(fontColor));

    const int rightPadding = padding + 15;
    int drawHeight = 0;
    int count = 0;

    for (QString item : listItems) {
        if (count >= offset / rowHeight) {
            QStringList list = item.split("=");
            QString resultStr = list.last();
            int resultWidth = fontMetrics().width(resultStr);
            QRect itemRect(padding, count * rowHeight - offset, rect().width(), rowHeight);

            if (resultWidth >= rect().width() / 2) {
                resultStr = fontMetrics().elidedText(resultStr, Qt::ElideRight, rect().width() / 2);
                resultWidth = fontMetrics().width(resultStr);
            }

            QString expStr = fontMetrics().elidedText(list.first(), Qt::ElideLeft, rect().width() - resultWidth - rightPadding * 2);

            // Check whether result text is digit.
            if (Utils::stringIsDigit(list.last())) {
                painter.setPen(QColor(fontColor));
            } else {
                painter.setPen(QColor(errorFontColor));
            }

            // Draw result text.
            painter.drawText(QRect(itemRect.x(), itemRect.y(), itemRect.width() - rightPadding, itemRect.height()), Qt::AlignVCenter | Qt::AlignRight, resultStr);

            // Draw expression text.
            painter.setPen(QColor(fontColor));
            painter.drawText(QRect(itemRect.x(), itemRect.y(), itemRect.width() - rightPadding - resultWidth, itemRect.height()), Qt::AlignVCenter | Qt::AlignRight, expStr + " = ");

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
            if (isShowScrollbar) {
                isShowScrollbar = false;
                update();
            }
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

    if (e->orientation() == Qt::Vertical) {
        qreal scrollStep = e->angleDelta().y() / 120.0;
        offset = adjustOffset(offset - scrollStep * rowHeight);
        isShowScrollbar = true;
        update();
    }

    e->accept();
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
    return (rect().height() * 1.0) / getItemsTotalHeight() * rect().height();
    //return qMax(scrollbarMinHeight, static_cast<int>(rect().height() * 1.0 / getItemsTotalHeight() * rect().height()));
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
