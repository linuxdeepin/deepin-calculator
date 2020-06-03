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
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include "dthememanager.h"

#include <DGuiApplicationHelper>

TextButton::TextButton(const QString &text, bool page, QWidget *parent)
    : DPushButton(text, parent)
    , m_effect(new QGraphicsDropShadowEffect(this))
{
    m_settings = DSettings::instance(this);
    int mode = m_settings->getOption("mode").toInt();
    mode == 0 ? setFixedSize(80, 58) : setFixedSize(67, 44);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");

    init();
    // connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &TextButton::init);
    m_isHover = m_isPress = false;
    m_page = page;

    m_effect->setOffset(0, 4);
    m_effect->setBlurRadius(12);
}

TextButton::~TextButton()
{
    // delete m_effect;
}

void TextButton::init()
{
    m_font.setPixelSize(18);
    m_font.setFamily("Noto Sans CJK SC");
    m_font.setStyleName("Light");
    m_font.setWeight(2);
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
    if (m_isPress == false) { //edit for bug-20492 20200416
        m_isHover = false;  //edit for bug-20508 20200414
        setDown(true);
        m_isPress = true;

        QTimer::singleShot(msec, this, [ = ] {
            setDown(false);
            m_isPress = false;
            update();
        });
    }
}

void TextButton::setButtonDown(bool down)
{
    m_Btnisdown = down;
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
        pa.setColor(DPalette::Light, QColor(0, 0, 0, 0.1 * 255));
        pa.setColor(DPalette::Dark, QColor(0, 0, 0, 0.1 * 255));
        pa.setColor(DPalette::ButtonText, QColor(0, 129, 255, 255));
        this->setPalette(pa);
    } else {
        DPalette pa = this->palette();
        pa.setColor(DPalette::Light, QColor(0, 0, 0, 0.5 * 255));
        pa.setColor(DPalette::Dark, QColor(0, 0, 0, 0.5 * 255));
        pa.setColor(DPalette::ButtonText, QColor(0, 129, 255, 255));
        this->setPalette(pa);
    }
    m_isPress = true;
    emit updateInterface();
    DPushButton::mousePressEvent(e);
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
//    clearFocus();
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

void TextButton::keyPressEvent(QKeyEvent *e)
{
    clearFocus();
    if (e->key() == Qt::Key_Left) {
        emit moveLeft();
        return;
    } else if (e->key() == Qt::Key_Right) {
        emit moveRight();
        return;
    } else if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
        return;
    else
        DPushButton::keyPressEvent(e);
}

void TextButton::paintEvent(QPaintEvent *e)
{
    int mode = m_settings->getOption("mode").toInt();
    mode == 0 ? setFixedSize(80, 58) : setFixedSize(67, 44);
    QRectF rect = this->rect();
    QRectF normal(rect.left() + 2, rect.top() + 2, rect.width() - 4, rect.height() - 4);
    QRectF hover(rect.left() + 3, rect.top() + 3, rect.width() - 6, rect.height() - 6);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setFont(m_font);
    //    QRectF textRect = painter.fontMetrics().boundingRect(this->text());
    //    textRect.moveCenter(rect.center());
    QRectF textRect = painter.fontMetrics().boundingRect(0, 0, rect.width(), rect.height(),
                                                         Qt::AlignCenter, this->text());
    //    textRect.moveCenter(rect.center());
    // QRectF
    // textRect(QPointF((rect.width()/2)-(textR.width()/2),(rect.height()/2)-(textR.height()/2)),textR.width(),textR.height());
    QColor actcolor = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color();//活动色
    QColor pressBrush, focus, hoverFrame, base, text, hoverbrush;
    QColor hoverShadow, focusShadow, normalShadow;
    QColor pressText = actcolor;
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        pressBrush = QColor(0, 0, 0, 0.1 * 255);
        focus = actcolor;
        hoverFrame = actcolor;
        hoverFrame.setAlphaF(0.2);
        if (m_page) {
            base = QColor(238, 238, 238);
            hoverbrush = QColor(238, 238, 238);
        } else {
            base = QColor("#FFFFFF");
            hoverbrush = QColor("#FFFFFF");
        }
        text = QColor("#000000");
        normalShadow = QColor(44, 167, 248, 0.4 * 255);
        hoverShadow = QColor(12, 155, 246, 0.1 * 255);
        focusShadow = QColor(0, 0, 0, 0.05 * 255);
    } else {
        pressBrush = QColor(0, 0, 0, 0.5 * 255);
        focus = actcolor;
        hoverFrame = actcolor;
        hoverFrame.setAlphaF(0.2);
        base = QColor("#303030");
        hoverbrush = QColor(255, 255, 255, 0.1 * 255);
        normalShadow = QColor(44, 167, 248, 0.4 * 255);
        hoverShadow = QColor(12, 155, 246, 0.1 * 255);
        focusShadow = QColor(0, 0, 0, 0.05 * 255);
        if (m_isHover)
            text = Qt::white;
        else
            text = QColor(224, 224, 224);
    }
    if (m_Btnisdown)
        text = actcolor;
    if (hasFocus()) {
        painter.setPen(Qt::NoPen);
        if (m_isPress) {
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        } else {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(focus);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundRect(normal, 25, 30);

            // painter.drawRoundRect(rect,10,10);
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
            painter.setBrush(QBrush(hoverbrush));
            painter.drawRoundRect(normal, 25, 30);

            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(hoverbrush));
//            painter.drawRoundRect(hover, 25, 30);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
            m_effect->setColor(hoverShadow);
            this->setGraphicsEffect(m_effect);
        } else if (m_isPress) {
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
            m_effect->setColor(focusShadow);
            this->setGraphicsEffect(m_effect);
        } else {
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
            m_effect->setColor(QColor(0, 0, 0, 0));
            this->setGraphicsEffect(m_effect);
        }
    }
}

void TextButton::focusOutEvent(QFocusEvent *e)
{
    emit updateInterface();
    QPushButton::focusOutEvent(e);
}
