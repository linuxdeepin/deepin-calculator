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

#ifndef MEMORYLISTWIDGET_H
#define MEMORYLISTWIDGET_H

#include <QListWidget>

/**
 * @brief 内存ListWidget
 */
class MemoryListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MemoryListWidget(QWidget *parent = nullptr);
    ~MemoryListWidget();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void itemselected(int row);

private:
    QPoint m_mousepoint;
    int m_clickrow = -1;

public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif // MEMORYLISTWIDGET_H
