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

#ifndef TESTITEMDELEGATE_H
#define TESTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

#include "memorylistwidget.h"
#include "memoryitemwidget.h"

/**
 * @brief 内存ListWidget委托
 */
class MemoryItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit MemoryItemDelegate(QObject *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TESTITEMDELEGATE_H
