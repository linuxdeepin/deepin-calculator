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

#ifndef PROLISTVIEW_H
#define PROLISTVIEW_H

#include <DWidget>
#include <DListView>
#include <QModelIndex>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QMouseEvent>

#include "prolistmodel.h"
#include "prolistdelegate.h"

DWIDGET_USE_NAMESPACE

class ProListView : public DListView
{
    Q_OBJECT

public:
    explicit ProListView(DWidget *parent = nullptr);
    ~ProListView();
    void contextMenuEvent(QContextMenuEvent *event);
    void showTextEditMenuByAltM(const QModelIndex &index);
    QModelIndex focusIndex() const;
    QModelIndex indexBeforeFocusOut() const;

public slots:
    void itemclicked(const QModelIndex &index, bool isspace);

protected:
//    void mouseMoveEvent(QMouseEvent *e);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *event);
Q_SIGNALS:
    void obtainingHistorical(const QModelIndex &index, bool isspace);

private:
    int m_focusrow = 1;
    int m_currentrow = 1;
    bool m_isMenuAltM = false; //是否是altM的菜单
};

#endif // PROLISTVIEW_H
