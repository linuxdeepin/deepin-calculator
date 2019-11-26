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
#include <QMouseEvent>

#include <DGuiApplicationHelper>

TextButton::TextButton(const QString &text, QWidget *parent)
    : DPushButton(text, parent)//,
      //m_effect(new QGraphicsDropShadowEffect(this))
{
    setFixedSize(80, 58);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");

    init();
    //connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &TextButton::init);
    m_isHover = m_isPress = false;
}

TextButton::~TextButton()
{
    //delete m_effect;
}

void TextButton::init()
{
    m_font.setPixelSize(18);
    m_font.setFamily("HelveticaNeue");
    m_font.setStyleName("Light");
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
    m_isPress = true;

    QTimer::singleShot(msec, this, [=] { setDown(false); m_isPress = false;});
}

void TextButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;
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
    m_isPress = true;
    DPushButton::mousePressEvent(e);
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;
    this->setPalette(m_palette);
    m_isPress = false;
    DPushButton::mouseReleaseEvent(e);
}

void TextButton::enterEvent(QEvent *e)
{
    m_font.setPixelSize(20);
    m_font.setStyleName("");
    m_isHover = true;
    DPushButton::enterEvent(e);
}

void TextButton::leaveEvent(QEvent *e)
{
    m_font.setPixelSize(18);
    m_font.setStyleName("Light");
    m_isHover = false;
    DPushButton::leaveEvent(e);
}

void TextButton::paintEvent(QPaintEvent *e)
{
    QRectF rect = this->rect();
    QRectF normal(rect.left() + 2,rect.top() + 2,rect.width() - 4,rect.height() - 4);
    QRectF hover(rect.left() + 3,rect.top() + 3,rect.width() - 6,rect.height() - 6);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setFont(m_font);
    QRectF textRect = painter.fontMetrics().boundingRect(this->text());
    textRect.moveCenter(rect.center());
    //QRectF textRect(QPointF((rect.width()/2)-(textR.width()/2),(rect.height()/2)-(textR.height()/2)),textR.width(),textR.height());
    QColor pressBrush,focus,hoverFrame,base,text;
    QColor pressText = QColor(0,129,255);
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        pressBrush = QColor(0,0,0,0.1*255);
        focus = QColor(0,129,255);
        hoverFrame = QColor(167,224,255);
        base = Qt::white;
        text = Qt::black;
    } else {
        pressBrush = QColor(0,0,0,0.5*255);
        focus = QColor(0,79,156);
        hoverFrame = QColor(0,79,156,0.5*255);
        base = QColor(48,48,48);
        if (m_isHover)
            text = Qt::white;
        else
            text = QColor(224,224,224);
    }
    if (hasFocus()) {
        painter.setPen(Qt::NoPen);
        if (m_isPress) {
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(normal,18,18);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        } else {
            QPen pen;
            pen.setColor(focus);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            QPainterPath path;
            path.addRoundedRect(rect,10,10);
            painter.fillPath(path,QBrush(focus));
            painter.drawPath(path);

            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(normal,18,18);

            //painter.drawRoundRect(rect,10,10);
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        }
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(base));
        if (m_isHover) {
            painter.setPen(QPen(hoverFrame));
            painter.setBrush(QBrush(hoverFrame));
            painter.drawRoundRect(normal,10,10);

            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(hover,10,10);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        } else if (m_isPress) {
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(normal,18,18);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        } else {
            painter.drawRoundRect(normal,18,18);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        }
    }
}
