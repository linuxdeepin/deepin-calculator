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

#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include "textbutton.h"

#include <DLabel>
#include <DSvgRenderer>

DWIDGET_USE_NAMESPACE

class IconButton : public TextButton
{
    Q_OBJECT

public:
    IconButton(QWidget *parent = nullptr);
    ~IconButton();

    void setIconUrl(const QString &normalFileName, const QString &hoverFileName, const QString &pressFileName);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    //void setIconSize(const int &size);

    DLabel *m_iconWidget;
    //DSvgRenderer *m_iconRenderer;

    int m_normalSize;
    int m_hoverSize;
    QString m_normalUrl;
    QString m_hoverUrl;
    QString m_pressUrl;
};

#endif
