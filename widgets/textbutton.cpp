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

#include "textbutton.h"
#include "dthememanager.h"
#include <QTimer>

DWIDGET_USE_NAMESPACE

TextButton::TextButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent),
      m_effect(new QGraphicsDropShadowEffect(this))
{
    setFixedSize(80, 60);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
    setGraphicsEffect(m_effect);

    init();
    hideShadow();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &TextButton::init);
}

TextButton::~TextButton()
{
    delete m_effect;
}

void TextButton::init()
{
    bool isEqual(text() == "=");

    if (DThemeManager::instance()->theme() == "light") {
        m_effect->setColor(QColor(12, 155, 246, 255 * 0.1));
        m_effect->setXOffset(0);
        m_effect->setYOffset(4);
        m_effect->setBlurRadius(12);

        if (isEqual) {
            m_effect->setColor(QColor(12, 155, 246, 255 * 0.8));
            m_effect->setBlurRadius(20);   
        }
    } else {
        m_effect->setColor(QColor(0, 0, 0, 255 * 0.1));
        m_effect->setXOffset(0);
        m_effect->setYOffset(4);
        m_effect->setBlurRadius(12);

        if (isEqual) {
            m_effect->setColor(QColor(12, 155, 246, 255 * 0.6));
            m_effect->setBlurRadius(30);
            m_effect->setXOffset(-2);
            m_effect->setYOffset(-4);            
        }
    }
}

void TextButton::showShadow()
{
    m_effect->setEnabled(true);
    raise();
}

void TextButton::hideShadow()
{
    m_effect->setEnabled(false);
}

void TextButton::animate(int msec)
{
    setDown(true);

    QTimer::singleShot(msec, this, [=] { setDown(false); });
}

void TextButton::mousePressEvent(QMouseEvent *e)
{
    hideShadow();

    QPushButton::mousePressEvent(e);
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
    showShadow();

    QPushButton::mouseReleaseEvent(e);
}

void TextButton::enterEvent(QEvent *e)
{
    showShadow();

    QPushButton::enterEvent(e);
}

void TextButton::leaveEvent(QEvent *e)
{
    hideShadow();

    QPushButton::leaveEvent(e);
}
