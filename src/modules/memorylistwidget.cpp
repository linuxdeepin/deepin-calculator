/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
*
* Maintainer: xiajing <xiajing@uniontech.com>
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
