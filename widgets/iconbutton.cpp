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

#include "iconbutton.h"
#include <QGridLayout>
#include <QDebug>
#include <QTimer>

IconButton::IconButton(QWidget *parent)
    : TextButton("", parent),
      m_iconWidget(new DLabel),
      m_iconRenderer(new DSvgRenderer)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_iconWidget,0,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    m_isHover = false;
    m_isPress = false;
}

IconButton::~IconButton()
{
}

void IconButton::setIconUrl(const QString &normalFileName, const QString &hoverFileName, const QString &pressFileName)
{
    m_normalUrl = normalFileName;
    m_hoverUrl = hoverFileName;
    m_pressUrl = pressFileName;

    m_iconRenderer->load(m_normalUrl);
    QPixmap pix(m_normalUrl);
    m_pixmap = pix;
    //setIcon(QIcon(m_pixmap));
    //setIconSize(QSize(30,30)*devicePixelRatioF());
}

void IconButton::animate(int msec)
{
    setDown(true);
    m_isPress = true;
    m_iconRenderer->load(m_pressUrl);
    QPixmap pixmap(m_pressUrl);
    m_pixmap = pixmap;

    QTimer::singleShot(msec, this, [=]
    { setDown(false);
      QPixmap pixmap(m_normalUrl);
      m_pixmap = pixmap;
      m_iconRenderer->load(m_normalUrl);
      m_isPress = false;
    });
}

void IconButton::mousePressEvent(QMouseEvent *e)
{
    m_iconRenderer->load(m_pressUrl);
    QPixmap pixmap(m_pressUrl);
    m_pixmap = pixmap;
    m_isPress = true;
    //pixmap.setDevicePixelRatio(devicePixelRatioF());
    //DPushButton::setIcon(QIcon(pixmap));

    TextButton::mousePressEvent(e);
}

void IconButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_iconRenderer->load(m_normalUrl);
    QPixmap pixmap(m_normalUrl);
    m_pixmap = pixmap;
    m_isPress = false;
    //pixmap.setDevicePixelRatio(devicePixelRatioF());
    //DPushButton::setIcon(QIcon(pixmap));

    TextButton::mouseReleaseEvent(e);
}

void IconButton::enterEvent(QEvent *e)
{
    m_iconRenderer->load(m_hoverUrl);
    QPixmap pixmap(m_hoverUrl);
    m_pixmap = pixmap;
    m_isHover = true;
    //pixmap.setDevicePixelRatio(devicePixelRatioF());
    //DPushButton::setIcon(QIcon(pixmap));

    TextButton::enterEvent(e);
}

void IconButton::leaveEvent(QEvent *e)
{
    m_iconRenderer->load(m_normalUrl);
    QPixmap pixmap(m_normalUrl);
    m_pixmap = pixmap;
    m_isHover = false;
    //pixmap.setDevicePixelRatio(devicePixelRatioF());
    //DPushButton::setIcon(QIcon(pixmap));

    TextButton::leaveEvent(e);
}

void IconButton::paintEvent(QPaintEvent *)
{
    QRectF frameRect = this->rect();
    QRectF rect(frameRect.left() + 2,frameRect.top() + 2,frameRect.width() - 4,frameRect.height() - 4);
    QRectF hover(frameRect.left() + 3,frameRect.top() + 3,frameRect.width() - 6,frameRect.height() - 6);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //m_pixmap = m_pixmap.scaled(m_pixmap.size() * devicePixelRatioF());
    QRectF pixRect = m_pixmap.rect();
    pixRect.moveCenter(rect.center());
    QColor pressBrush,focus,hoverFrame,base;
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        pressBrush = QColor(0,0,0,0.1*255);
        focus = QColor(0,129,255);
        hoverFrame = QColor(167,224,255);
        base = Qt::white;
    } else {
        pressBrush = QColor(0,0,0,0.5*255);
        focus = QColor(0,79,156);
        hoverFrame = QColor(0,79,156,0.5*255);
        base = QColor(48,48,48);
    }
    if (hasFocus()) {
        if (m_isPress) {
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(rect,30,40);
        } else {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(rect,30,40);
            QPen pen;
            pen.setColor(focus);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundRect(rect,30,40);
        }
    } else {
        if (m_isHover) {
            painter.setPen(QPen(hoverFrame));
            painter.setBrush(QBrush(hoverFrame));
            painter.drawRoundRect(rect,10,10);

            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(hover,10,10);
        } else if (m_isPress) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(rect,10,10);
        } else {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(rect,30,40);
        }
        //painter.setPen(QPen(hoverFrame));
        //painter.setBrush(Qt::NoBrush);
        //painter.drawRoundRect(rect,30,40);
    }
    //painter.drawPixmap(pixRect.topLeft(),m_pixmap);
    m_iconRenderer->render(&painter,pixRect);
}

/*void IconButton::setIconSize(const int &size)
{
    const int scaledSize = size * devicePixelRatioF();
    const QSize iconSize(scaledSize, scaledSize);
    const QImage image = m_iconRenderer->toImage(iconSize);

    QPixmap pix;
    pix.convertFromImage(image);
    pix.setDevicePixelRatio(devicePixelRatioF());

    //m_iconWidget->setAlignment(Qt::AlignCenter);
    //m_iconWidget->setPixmap(pix);
    this->DPushButton::setIcon(QIcon(pix));
    //m_iconWidget->setFixedSize(iconSize);
}*/
