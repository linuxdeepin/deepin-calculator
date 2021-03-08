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

#ifndef PROLISTDELEGATE_H
#define PROLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

class ProListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ProListDelegate(QObject *parent = nullptr);
    ~ProListDelegate();

public slots:
    void setThemeType(int type);
    void currentfocusindex(QModelIndex index);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawFocusStatus(QPainter *painter, const QStyleOptionViewItem &option) const;
signals:
    void obtainingHistorical(const QModelIndex &index);

private:
    int m_themeType = 0;
    QModelIndex m_focusindex;
};

#endif // PROLISTDELEGATE_H
