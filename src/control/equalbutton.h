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

#ifndef EQUALBUTTON_H
#define EQUALBUTTON_H

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <DPushButton>
#include <DSuggestButton>
#include <DPalette>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class EqualButton : public DSuggestButton
{
    Q_OBJECT
public:
    EqualButton(const QString &text = QString(), QWidget *parent = nullptr);
    ~EqualButton() override;

    void init();
    //void showShadow();
    //void hideShadow();
    void animate(bool isspace = false, int msec = 100);
signals:
    void moveLeft();
    void moveRight();
    void focus(int direction); //direction 0-上　1-下　2-左　3-右
    void space();

public:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    QGraphicsDropShadowEffect *m_effect;
    DPalette m_palette;
    bool m_isHover;
    bool m_isPress;
    QFont m_font;
    bool m_isacting = false; //规避鼠标在button内release时出现foucus状态的框
};

#endif // EQUALBUTTON_H
