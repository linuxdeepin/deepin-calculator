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

IconButton::IconButton(QWidget *parent)
    : TextButton("", parent),
      m_iconWidget(new QSvgWidget)
{
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(m_iconWidget);
}

IconButton::~IconButton()
{
}

void IconButton::setIcon(const QString &fileName)
{
    m_iconWidget->load(fileName);
    m_iconWidget->setFixedSize(23, 23);
}

void IconButton::mousePressEvent(QMouseEvent *e)
{
    m_iconWidget->setFixedSize(23, 23);

    TextButton::mousePressEvent(e);
}

void IconButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_iconWidget->setFixedSize(26, 26);

    TextButton::mouseReleaseEvent(e);
}

void IconButton::enterEvent(QEvent *e)
{
    m_iconWidget->setFixedSize(26, 26);

    TextButton::enterEvent(e);
}

void IconButton::leaveEvent(QEvent *e)
{
    m_iconWidget->setFixedSize(23, 23);

    TextButton::leaveEvent(e);
}
