/*
* Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
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

#include "programmerarrowdelegate.h"

#include <DStyleHelper>
#include <DApplicationHelper>
#include "memorylistwidget.h"
#include "programmeritemwidget.h"

ProgrammerArrowDelegate::ProgrammerArrowDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ProgrammerArrowDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //边框绘制
    MemoryListWidget *dragWidget = qobject_cast<MemoryListWidget *>(option.styleObject);
    QRectF rect = dragWidget->rect(); //获取memorylistwidget.rect绘制背景色
    painter->setRenderHint(QPainter::Antialiasing, true);
    int type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor("#F8F8F8"));
        painter->drawRect(rect);
    } else {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor("#252525"));
        painter->drawRect(rect);
    }
    if (dragWidget->hasFocus()) {
        QRectF itemrect(dragWidget->visualItemRect(dragWidget->currentItem()));
        QColor color(Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color().name());
        painter->setBrush(color);
        painter->drawRect(itemrect); //背景填充
    }
    QStyledItemDelegate::paint(painter, option, index);
}

void ProgrammerArrowDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
