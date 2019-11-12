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

IconButton::IconButton(QWidget *parent)
    : TextButton("", parent),
      m_iconWidget(new DLabel),
      m_iconRenderer(new DSvgRenderer)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_iconWidget, 0, Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

IconButton::~IconButton()
{
}

void IconButton::setIconUrl(const QString &normalFileName, const QString &hoverFileName, const QString &pressFileName)
{
    m_normalUrl = normalFileName;
    m_hoverUrl = hoverFileName;
    m_pressUrl = pressFileName;

    QPixmap pix(m_normalUrl);
    pix = pix.scaled(600, 600);
    pix.setDevicePixelRatio(devicePixelRatioF());
    setIcon(QIcon(pix));
    setIconSize(QSize(30, 30));
}

void IconButton::setIconStateSizes(int normalSize, int hoverSize)
{
    m_normalSize = normalSize;
    m_hoverSize = hoverSize;
}

void IconButton::mousePressEvent(QMouseEvent *e)
{
    QPixmap pixmap(m_pressUrl);
    pixmap.setDevicePixelRatio(devicePixelRatioF());
    DPushButton::setIcon(QIcon(pixmap));

    TextButton::mousePressEvent(e);
}

void IconButton::mouseReleaseEvent(QMouseEvent *e)
{
    QPixmap pixmap(m_normalUrl);
    pixmap.setDevicePixelRatio(devicePixelRatioF());
    DPushButton::setIcon(QIcon(pixmap));

    TextButton::mouseReleaseEvent(e);
}

void IconButton::enterEvent(QEvent *e)
{
    QPixmap pixmap(m_hoverUrl);
    pixmap.setDevicePixelRatio(devicePixelRatioF());
    DPushButton::setIcon(QIcon(pixmap));

    TextButton::enterEvent(e);
}

void IconButton::leaveEvent(QEvent *e)
{
    QPixmap pixmap(m_normalUrl);
    pixmap.setDevicePixelRatio(devicePixelRatioF());
    DPushButton::setIcon(QIcon(pixmap));

    TextButton::leaveEvent(e);
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
