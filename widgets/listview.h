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

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>

class ListView : public QWidget
{
    Q_OBJECT

public:
    ListView(QWidget *parent = nullptr);
    ~ListView();

    void initTheme();
    void addItem(const QString &text);
    void clearItems();
    void scrollToBottom();
    int getItemsCount();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void leaveEvent(QEvent *);
    int getItemsTotalHeight();
    int getScrollbarHeight();
    int getScrollbarX();
    int getScrollbarY();
    int adjustOffset(const int &offset);

private:
    QList<QString> m_listItems;
    int m_rowHeight;
    int m_padding;
    int m_offset;
    int m_scrollbarMinHeight;
    bool m_isPress;
    bool m_isShowScrollbar;
    QString m_backgroundColor;
    QString m_fontColor;
    QString m_errorFontColor;
    QString m_scrollbarColor;
};

#endif
