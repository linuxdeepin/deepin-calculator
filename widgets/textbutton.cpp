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
#include <QPainter>

#include <DGuiApplicationHelper>

TextButton::TextButton(const QString &text, QWidget *parent)
    : DPushButton(text, parent)//,
      //m_effect(new QGraphicsDropShadowEffect(this))
{
    setFixedSize(76, 53);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
    m_isHover = false;
    m_isPress = false;

    init();
    //hideShadow();

    //connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &TextButton::init);
}

TextButton::~TextButton()
{
    //delete m_effect;
}

void TextButton::init()
{
    /*if (DThemeManager::instance()->theme() == "light") {
        m_effect->setColor(QColor(12,155,246));
        m_effect->setXOffset(0);
        m_effect->setYOffset(4);
        m_effect->setBlurRadius(12);
    } else {
        m_effect->setColor(QColor(0, 0, 0, 255 * 0.1));
        m_effect->setXOffset(0);
        m_effect->setYOffset(4);
        m_effect->setBlurRadius(12);
    }*/
    QFont font;
    font.setPixelSize(18);
    this->setFont(font);
    DPalette pl = this->palette();
    pl.setColor(DPalette::ButtonText, Qt::black);
    this->setPalette(pl);
}

/*void TextButton::showShadow()
{
    m_effect->setEnabled(true);
    raise();
}

void TextButton::hideShadow()
{
    m_effect->setEnabled(false);
}*/

void TextButton::animate(int msec)
{
    setDown(true);

    QTimer::singleShot(msec, this, [=] { setDown(false); });
}

void TextButton::mousePressEvent(QMouseEvent *e)
{
    setFocus();
    m_palette = this->palette();
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        DPalette pa = this->palette();
        pa.setColor(DPalette::Light, QColor(0,0,0,0.1*255));
        pa.setColor(DPalette::Dark, QColor(0,0,0,0.1*255));
        pa.setColor(DPalette::ButtonText, QColor(0,129,255,255));
        this->setPalette(pa);
    }
    else {
        DPalette pa = this->palette();
        pa.setColor(DPalette::Light, QColor(0,0,0,0.5*255));
        pa.setColor(DPalette::Dark, QColor(0,0,0,0.5*255));
        pa.setColor(DPalette::ButtonText, QColor(0,129,255,255));
        this->setPalette(pa);
    }
    DPushButton::mousePressEvent(e);
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
    /*int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        DPalette pa = this->palette();
        pa.setColor(DPalette::Light, Qt::white);
        pa.setColor(DPalette::Dark, Qt::white);
        pa.setColor(DPalette::ButtonText, Qt::black);
        this->setPalette(pa);
    }
    else {
        DPalette pa = this->palette();
        pa.setColor(DPalette::Light, QColor(48,48,48));
        pa.setColor(DPalette::Dark, QColor(48,48,48));
        pa.setColor(DPalette::ButtonText, QColor(224,224,224));
        this->setPalette(pa);
    }*/
    this->setPalette(m_palette);
    DPushButton::mouseReleaseEvent(e);
}

void TextButton::enterEvent(QEvent *e)
{
    QFont font = this->font();
    font.setPixelSize(20);
    this->setFont(font);
    DPushButton::enterEvent(e);
}

void TextButton::leaveEvent(QEvent *e)
{
    QFont font = this->font();
    font.setPixelSize(18);
    this->setFont(font);
    DPushButton::leaveEvent(e);
}
