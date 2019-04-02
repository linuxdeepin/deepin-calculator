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
#include <QHBoxLayout>

#include <QDebug>

IconButton::IconButton(int normalSize, int hoverSize, QWidget *parent)
    : TextButton("", parent),
      m_iconWidget(new QLabel),
      m_iconRenderer(new DSvgRenderer),
      m_normalSize(normalSize),
      m_hoverSize(hoverSize)
{  
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(m_iconWidget);
}

IconButton::~IconButton()
{
}

void IconButton::setIcon(const QString &fileName)
{
    m_iconRenderer->load(fileName);
    setIconSize(m_normalSize);
}

void IconButton::setIconStateSizes(int normalSize, int hoverSize)
{
    m_normalSize = normalSize;
    m_hoverSize = hoverSize;
}

void IconButton::mousePressEvent(QMouseEvent *e)
{
    setIconSize(m_normalSize);

    TextButton::mousePressEvent(e);
}

void IconButton::mouseReleaseEvent(QMouseEvent *e)
{
    setIconSize(m_hoverSize);

    TextButton::mouseReleaseEvent(e);
}

void IconButton::enterEvent(QEvent *e)
{
    setIconSize(m_hoverSize);

    TextButton::enterEvent(e);
}

void IconButton::leaveEvent(QEvent *e)
{
    setIconSize(m_normalSize);

    TextButton::leaveEvent(e);
}

void IconButton::setIconSize(const int &size)
{
    const int scaledSize = size * devicePixelRatioF();
    const QSize iconSize(scaledSize, scaledSize);
    const QImage image = m_iconRenderer->toImage(iconSize);

    QPixmap pix;
    pix.convertFromImage(image);
    pix.setDevicePixelRatio(devicePixelRatioF());

    m_iconWidget->setPixmap(pix);
    m_iconWidget->setFixedSize(iconSize);
}
