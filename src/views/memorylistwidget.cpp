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

#include "programmeritemwidget.h"

DGUI_USE_NAMESPACE

MemoryListWidget::MemoryListWidget(QWidget *parent, bool isarrowlist)
    : QListWidget(parent)
{
//    DPalette pal = this->palette();
//    pal.setColor(DPalette::Light, QColor(248, 248, 248));
//    this->setPalette(pal);
//    this->setAttribute(Qt::WA_StyledBackground, false);
//    DPalette pal = this->palette();
//    pal.setColor(DPalette::Light, QColor(248, 248, 248));
//    this->setPalette(pal);
    setFocusPolicy(Qt::NoFocus); //防止内存无内容时被focus
    if (isarrowlist) {
        setMouseTracking(true);
        installEventFilter(this);
    }
}

/**
 * @brief 获取当前点击row;
 */
void MemoryListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        m_mousemovepoint = event->globalPos();
        m_mousepoint = event->pos();
        if (this->itemAt(m_mousepoint)) {
            m_clickrow = this->row(this->itemAt(m_mousepoint));
//            this->itemAt(mousepoint)->setBackground(QBrush(QColor(0, 0, 0)));
        }
    }

    QListWidget::mousePressEvent(event);
}

/**
 * @brief 发送内存被选行信号
 */
void MemoryListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->itemAt(event->pos()) && this->row(this->itemAt(event->pos())) == m_clickrow) {
        emit itemselected(m_clickrow);
    }
    m_clickrow = -1;
    QListWidget::mouseReleaseEvent(event);
}

/**
 * @brief focus时上下左右与空格键的信号
 * @param 0-上　1-下　2-左　3-右
 */
void MemoryListWidget::keyPressEvent(QKeyEvent *e)
{
    bool ispressalt = e->modifiers() == Qt::AltModifier;
    switch (e->key()) {
    case Qt::Key_Up:
        emit focus(0);
        break;
    case Qt::Key_Down:
        emit focus(1);
        break;
    case Qt::Key_Left:
        emit focus(2);
        break;
    case Qt::Key_Right:
        emit focus(3);
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit space();
        break;
    case Qt::Key_M:
        if (ispressalt)
            emit altAndM();
        break;
    default:
        QWidget::keyPressEvent(e);
        break;
    }
}

/**
 * @brief 焦点进入时设置焦点在第一条记录
 */
void MemoryListWidget::focusInEvent(QFocusEvent *event)
{
//    setCurrentRow(0);
    //暂定
//    if (currentRow() == -1)
//        setCurrentRow(0);
    scrollToItem(this->item(currentRow()));
    QWidget::focusInEvent(event);
}

bool MemoryListWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        if (this->hasFocus())
            emit mousemoving();
        return true;
    }
    return QListWidget::eventFilter(obj, event);
}

void MemoryListWidget::oneItemFocused()
{
    for (int i = 0; i < 4; i++) {
        static_cast<ProgrammerItemWidget *>(itemWidget(item(i)))->cleanHoverState();
    }
}

MemoryListWidget::~MemoryListWidget()
{
}
