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
#include "../dsettings.h"

#include <QPaintEvent>

#include <DLabel>
#include <DSvgRenderer>
#include <DGuiApplicationHelper>
#include <QDomElement>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class IconButton : public TextButton
{
    Q_OBJECT

public:
    IconButton(QWidget *parent = nullptr, int b = 0, bool page = false);
    ~IconButton();

    void setIconUrl(const QString &normalFileName, const QString &hoverFileName, const QString &pressFileName, int mode = 0);
    void animate(int msec = 100);
    void settooltip(bool b);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
    bool event(QEvent *);

private:
    //void setIconSize(const int &size);

    DLabel *m_iconWidget;
    DSvgRenderer *m_iconRenderer;
    DSettings *m_settings;

    int m_normalSize;
    int m_hoverSize;
    bool m_isHover;
    bool m_isPress;
    QPixmap m_pixmap;
    QString m_normalUrl;
    QString m_hoverUrl;
    QString m_pressUrl;
    int m_mode = 0; //0-四则运算符 1-退格符 2-退格符press 3-根号 4-根号press 5-deg 6-degpress
    QString m_currentUrl;
    int m_buttonStatus = 0; //0-normal 1-hover 2-press
    bool m_isEmptyBtn; //内存中的清空按钮
    int m_themetype;
    int m_page;
    bool m_isHistorybtn = false;
    QString cleartooltip = QString();
private:
    void SetAttrRecur(QDomElement elem, QString strtagname, QString strattr, QString strattrval);
    void drawCenterPixMap(QPainter &painter);
signals:
    void isclicked();
};

#endif
