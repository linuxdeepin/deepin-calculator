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

#ifndef ARROWRECTANGLE_H
#define ARROWRECTANGLE_H

#include <DArrowRectangle>
#include <QWidget>
DWIDGET_USE_NAMESPACE

class ArrowRectangle : public DArrowRectangle
{
    Q_OBJECT
public:
    explicit ArrowRectangle(ArrowDirection direction, FloatMode floatMode, QWidget *parent = nullptr);
    ~ArrowRectangle();
    bool eventFilter(QObject *obj, QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void mouseMoveToClearFocus();
signals:
    void hidearrowrectangle(bool isesc = false);//判断是否为esc退出
};

#endif // ARROWRECTANGLE_H
