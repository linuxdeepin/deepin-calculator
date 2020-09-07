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

#include <QPaintEvent>
#include <QDomElement>
#include <QSvgRenderer>
#include <DLabel>
#include <DGuiApplicationHelper>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class IconButton : public TextButton
{
    Q_OBJECT

public:
    /**
     * @brief IconButton
     * @param parent
     * @param b 1-垃圾桶 2-科学计算器titlebar历史区按钮
     * @param page 科学模式第二页的IconButton
     */
    IconButton(QWidget *parent = nullptr, int b = 0, bool page = false);
    ~IconButton();

    void setIconUrl(const QString &normalFileName, const QString &hoverFileName, const QString &pressFileName, int mode = 0);
    void animate(bool isspace = false, int msec = 100);
    void showtooltip(bool ismemory); //ismemory-true-内存区垃圾桶 false-历史区垃圾桶

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *e);

private:
    //void setIconSize(const int &size);

//    DLabel *m_iconWidget;
    QGraphicsDropShadowEffect *m_effect;

//    DSvgRenderer *m_iconRenderer;
    QSvgRenderer *m_iconRenderer; //改用QSvgRenderer

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
    bool m_page;
    bool m_isHistorybtn = false; //打开历史记录的按钮，暂时保存
    QString m_cleartooltip = QString();
    bool m_isacting = false; //规避鼠标在button内release时出现foucus状态的框
private:
    void SetAttrRecur(QDomElement elem, QString strtagname, QString strattr, QString strattrval);
    void drawCenterPixMap(QPainter &painter);
signals:
    void isclicked();
};

#endif
