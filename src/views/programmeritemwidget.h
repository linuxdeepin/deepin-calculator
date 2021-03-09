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

#ifndef PROGRAMMERITEMWIDGET_H
#define PROGRAMMERITEMWIDGET_H

#include <QDebug>
#include <QWidget>
#include <DIconButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <DGuiApplicationHelper>
#include <QSvgRenderer>

DWIDGET_USE_NAMESPACE

class ProgrammerItemWidget : public QWidget
{
    Q_OBJECT
public:
    ProgrammerItemWidget(QString label, const int width, const QString path, QWidget *parent = nullptr);
    ProgrammerItemWidget(QString label, QWidget *parent = nullptr);
    ~ProgrammerItemWidget();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
    void cleanHoverState();
    void cleanFocusState();
    void isMarkHide(bool ishide);

public slots:
    void themetypechanged(int type);
    void setFocus();
    void resetWidth(const int width); // 重设宽度，用于字体改变的时候

private:
    void drawMark(QPainter *painter) const;
    void initMark();
    void drawIcon(QPainter *painter) const;

private:
    DIconButton *m_iconbtn;
    QLabel *m_label;
    bool m_ispress = false;
    bool m_ishover = false;
    bool m_isfocus = false;
    int m_themetype = 1;
    QSvgRenderer *m_iconRenderer;
    bool m_isshift = false; //判断是数据的列表还是移位的列表
    int m_width = 0;//宽度

    //画一个勾
    QPoint m_markstart;
    QPoint m_markmid;
    QPoint m_markend;
    QVector<QPoint> m_pointVec;
    QPen m_markPen;
    bool m_isMarkHide = true;

    //画一张图
    QPixmap m_pixmap;
    QString m_normalUrl = QString();
    QString m_hoverUrl;
    QString m_pressUrl;
    QString m_currentUrl = QString();
};

#endif // PROGRAMMERITEMWIDGET_H
