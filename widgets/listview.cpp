/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "listview.h"
#include "dthememanager.h"
#include "utils.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

DWIDGET_USE_NAMESPACE

ListView::ListView(QWidget *parent) 
    : QWidget(parent)
{
    m_rowHeight = 35;
    m_padding = 10;
    m_offset = 0;
    m_scrollbarMinHeight = 30;
    m_isPress = false;
    m_isShowScrollbar = false;

    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::NoFocus);
    setMouseTracking(true);
    setMinimumHeight(105);
    initTheme();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &ListView::initTheme);
}

ListView::~ListView()
{
}

void ListView::initTheme()
{
    if (DThemeManager::instance()->theme() == "light") {
        m_backgroundColor = "#FBFBFB";
        m_fontColor = "#636363";
        m_errorFontColor = "#F37D54";
        m_scrollbarColor = "#000000";
    } else {
        m_backgroundColor = "#111111";
        m_fontColor = "#C3C3C3";
        m_errorFontColor = "#F37D54";
        m_scrollbarColor = "#FFFFFF";
    }
}

void ListView::addItem(const QString &text)
{
    for (const QString &itemStr : m_listItems) {
        if (itemStr == text) {
            m_listItems.removeAt(m_listItems.indexOf(itemStr));
        }
    }

    m_listItems << text;
    scrollToBottom();
}

void ListView::clearItems()
{
    m_listItems.clear();

    scrollToBottom();
}

void ListView::scrollToBottom()
{
    m_offset = getItemsTotalHeight() - rect().height();

    update();
}

int ListView::getItemsCount()
{
    return m_listItems.count();
}

void ListView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw Items.
    painter.setFont(QFont("Noto Sans CJK SC"));
    painter.setPen(QColor(m_fontColor));

    const int rightPadding = m_padding + 15;
    int drawHeight = 0;
    int count = 0;

    for (QString item : m_listItems) {
        if (count >= m_offset / m_rowHeight) {
            QStringList list = item.split("=");
            QString resultStr = list.last();
            int resultWidth = fontMetrics().width(resultStr);
            QRect itemRect(m_padding, count * m_rowHeight - m_offset, rect().width(), m_rowHeight);

            if (resultWidth > rect().width() / 1.3) {
                resultStr = fontMetrics().elidedText(resultStr, Qt::ElideRight, rect().width() / 1.3);
                resultWidth = fontMetrics().width(resultStr);
            }

            QString expStr = fontMetrics().elidedText(list.first(), Qt::ElideLeft,
                                                      rect().width() - resultWidth - rightPadding * 2);

            // Check whether result text is digit.
            if (Utils::stringIsDigit(list.last())) {
                painter.setPen(QColor(m_fontColor));
            } else {
                painter.setPen(QColor(m_errorFontColor));
            }

            // Draw result text.
            painter.drawText(QRect(itemRect.x(),
                                   itemRect.y(),
                                   itemRect.width() - rightPadding,
                                   itemRect.height()),
                             Qt::AlignVCenter | Qt::AlignRight, resultStr);

            // Draw expression text.
            painter.setPen(QColor(m_fontColor));
            painter.drawText(QRect(itemRect.x(),
                                   itemRect.y(),
                                   itemRect.width() - rightPadding - resultWidth,
                                   itemRect.height()), Qt::AlignVCenter | Qt::AlignRight, expStr + " = ");

            drawHeight += m_rowHeight;

            if (drawHeight > rect().height())
                break;
        }

        ++count;
    }

    // Draw scrollbar
    if (!m_isShowScrollbar)
        return;

    painter.setBrush(QColor(m_scrollbarColor));

    if (m_isPress) {
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
    bool isShowbar = getItemsTotalHeight() > rect().height();

    if (isShowbar) {
        if (m_isPress) {
            m_offset = adjustOffset((e->y() - getScrollbarHeight() / 2) / (rect().height() * 1.0) * getItemsTotalHeight());
            m_isShowScrollbar = true;
            update();
        } else if (e->x() > getScrollbarX()) {
            m_isShowScrollbar = true;
            update();
        } else {
            if (m_isShowScrollbar) {
                m_isShowScrollbar = false;
                update();
            }
        }
    }

    if (!isShowbar || e->x() < getScrollbarX() && !m_isPress) {
        QWidget::mouseMoveEvent(e);
    }
}

void ListView::mousePressEvent(QMouseEvent *e)
{
    if (getItemsTotalHeight() > rect().height()) {
        if (e->x() > getScrollbarX()) {
            m_isPress = true;
            m_offset = adjustOffset((e->y() - getScrollbarHeight() / 2) / (rect().height() * 1.0) * getItemsTotalHeight());
            update();
        }
    }
}

void ListView::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_isPress) {
        m_isPress = false;
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
        m_offset = adjustOffset(m_offset - scrollStep * m_rowHeight);
        m_isShowScrollbar = true;
        update();
    }

    e->accept();
}

void ListView::leaveEvent(QEvent *)
{
    m_isShowScrollbar = false;

    update();
}

int ListView::getItemsTotalHeight()
{
    return m_listItems.count() * m_rowHeight;
}

int ListView::getScrollbarHeight()
{
    return qMax(m_scrollbarMinHeight, static_cast<int>(rect().height() * 1.0 / getItemsTotalHeight() * rect().height()));
}

int ListView::getScrollbarX()
{
    return rect().width() - 10;
}

int ListView::getScrollbarY()
{
    int y = m_offset / (getItemsTotalHeight() * 1.0) * rect().height();

    return qMin(y, rect().height() - getScrollbarHeight());
}

int ListView::adjustOffset(const int &offset)
{
    return qMax(0, qMin(offset, getItemsTotalHeight() - rect().height()));
}
