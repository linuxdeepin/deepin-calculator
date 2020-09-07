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

#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <DPushButton>
#include <DPalette>
#include <DGuiApplicationHelper>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class TextButton : public DPushButton
{
    Q_OBJECT

public:
    TextButton(const QString &text = QString(), bool page = false, QWidget *parent = nullptr); //page-是否是科学计算器中两页中的按钮
    ~TextButton();

    void init();
    void animate(bool isspace = false, int msec = 100);
    void setButtonDown(bool down);
signals:
    void moveLeft();
    void moveRight();
    void updateInterface();
    void mouseRelease();
    void focus(int direction); //direction 0-上　1-下　2-左　3-右
    void space();

public:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *e);
    void focusOutEvent(QFocusEvent *);
    void keyPressEvent(QKeyEvent *e);

private:
    void paintspecialbtn(QPainter &painter, QRectF rect, QRectF textRect);

private:
    QGraphicsDropShadowEffect *m_effect;

    DPalette m_palette;
    bool m_isHover;
    bool m_isPress;
    QFont m_font;
    bool m_page; //m_page-是否是科学计算器中两页中的按钮
    bool m_Btnisdown = false;
    bool m_isacting = false; //规避鼠标在button内release时出现foucus状态的框
    int m_btn = 0; //1-2ⁿᵈ, 2-xʸ, 3-10ˣ, 4-2ˣ, 5-eˣ, 6-arcsin, 7-arccos, 8-arctan, 9-arccot
};

#endif
