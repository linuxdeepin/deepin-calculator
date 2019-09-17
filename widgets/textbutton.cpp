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

#include <DPalette>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

TextButton::TextButton(const QString &text, QWidget *parent)
    : DPushButton(text, parent),
      m_effect(new QGraphicsDropShadowEffect(this))
{
    setFixedSize(76, 58);
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
    if (isEqual) {
        DPalette pa = this->palette();
        pa.setColor(DPalette::WindowText, Qt::white);
        this->setPalette(pa);
    }

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
    QFont font;
    font.setPixelSize(18);
    this->setFont(font);
    DPalette pl = this->palette();
    pl.setColor(DPalette::ButtonText, Qt::black);
    this->setPalette(pl);
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
    //hideShadow();

    DPushButton::mousePressEvent(e);
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
    //showShadow();

    DPushButton::mouseReleaseEvent(e);
}

void TextButton::enterEvent(QEvent *e)
{
    //showShadow();

    DPushButton::enterEvent(e);
}

void TextButton::leaveEvent(QEvent *e)
{
    //hideShadow();

    DPushButton::leaveEvent(e);
}
