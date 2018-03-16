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

#include "simplelistview.h"
#include <QMouseEvent>
#include <QDebug>

SimpleListView::SimpleListView(QWidget *parent)
    : QListView(parent)
{
    setVerticalScrollMode(ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);
    setAutoScroll(false);
    setFixedHeight(105);

    setViewportMargins(0, 0, -verticalScrollBar()->sizeHint().width(), 0);
}

SimpleListView::~SimpleListView()
{
}

void SimpleListView::mouseMoveEvent(QMouseEvent *e)
{
    if (e->x() < width() - 12) {
        QWidget::mouseMoveEvent(e);
    } else {
        QListView::mouseMoveEvent(e);
    }
}
