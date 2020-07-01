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
#include <QToolTip>

IconButton::IconButton(QWidget *parent, int b, bool page)
    : TextButton("", parent),
//      m_iconWidget(new DLabel),
      m_iconRenderer(new DSvgRenderer(this))
{
    m_settings = DSettings::instance(this);
    int mode = m_settings->getOption("mode").toInt();
    if (mode == 0)
        setFixedSize(80, 58);
    if (b == 1)
        setFixedSize(40, 40);
    if (b == 2) {
        m_isHistorybtn = true;
        setFixedSize(50, 50);
    }
//    QGridLayout *layout = new QGridLayout(this);
//    layout->addWidget(m_iconWidget, 0, Qt::AlignCenter);
//    layout->setContentsMargins(0, 0, 0, 0);
//    setLayout(layout);
    m_isHover = false;
    m_isPress = false;
    m_isEmptyBtn = (b == 1);
    m_page = page;
}

IconButton::~IconButton()
{
}

void IconButton::setIconUrl(const QString &normalFileName, const QString &hoverFileName, const QString &pressFileName, int mode)
{
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    m_themetype = type;
    m_normalUrl = normalFileName;
    m_hoverUrl = hoverFileName;
    m_pressUrl = pressFileName;
    m_currentUrl = normalFileName;

    m_currentUrl = m_normalUrl;
    m_buttonStatus = 0;
    //setIcon(QIcon(m_pixmap));
    //setIconSize(QSize(30,30)*devicePixelRatioF());
    m_mode = mode;
}

void IconButton::animate(int msec)
{
    if (m_isPress == false) { //edit for bug-20492 20200416
        m_isHover = false;  //edit for bug-20508 20200414
        setDown(true);
        m_isPress = true;
        m_currentUrl = m_pressUrl;
        m_buttonStatus = 2;
        if (m_mode == 1)
            m_mode = 2;
        if (m_mode == 3)
            m_mode = 4;
        if (m_mode == 5)
            m_mode = 6;

        QTimer::singleShot(msec, this, [ = ] {
            setDown(false);
            m_currentUrl = m_normalUrl;
            m_buttonStatus = 0;
            if (m_mode == 2)
                m_mode = 1;
            if (m_mode == 4)
                m_mode = 3;
            if (m_mode == 5)
                m_mode = 6;
            m_isPress = false;
            update();
        });
    }
}

void IconButton::showtooltip(bool ismemory)
{
    if (ismemory == true) {
        m_cleartooltip = tr("Clear all memory");
        this->setToolTip(m_cleartooltip);
    } else {
        m_cleartooltip = tr("Clear history");
        this->setToolTip(m_cleartooltip);
    }
}

void IconButton::mousePressEvent(QMouseEvent *e)
{
    m_currentUrl = m_pressUrl;
    m_buttonStatus = 2;
    if (m_mode == 1)
        m_mode = 2;
    if (m_mode == 3)
        m_mode = 4;
    if (m_mode == 5)
        m_mode = 6;
    m_isPress = true;
    //pixmap.setDevicePixelRatio(devicePixelRatioF());
    //DPushButton::setIcon(QIcon(pixmap));

    TextButton::mousePressEvent(e);
}

void IconButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_isHistorybtn)
        clearFocus();
    m_currentUrl = m_normalUrl;
//    m_buttonStatus = 0;
    if (m_mode == 2)
        m_mode = 1;
    if (m_mode == 4)
        m_mode = 3;
    if (m_mode == 6)
        m_mode = 5;
    if (m_isPress == true && this->rect().contains(e->pos())) {
        m_currentUrl = m_hoverUrl;
        m_buttonStatus = 1;
        emit isclicked();
    } else {
        m_buttonStatus = 0;
    }
    m_isPress = false;

    TextButton::mouseReleaseEvent(e);
}

void IconButton::enterEvent(QEvent *e)
{
    m_currentUrl = m_hoverUrl;
    m_buttonStatus = 1;
    m_isHover = true;
    //pixmap.setDevicePixelRatio(devicePixelRatioF());
    //DPushButton::setIcon(QIcon(pixmap));

    TextButton::enterEvent(e);
}

void IconButton::leaveEvent(QEvent *e)
{
    m_currentUrl = m_normalUrl;
    m_buttonStatus = 0;
    m_isHover = false;
    //pixmap.setDevicePixelRatio(devicePixelRatioF());
    //DPushButton::setIcon(QIcon(pixmap));

    TextButton::leaveEvent(e);
}

void IconButton::paintEvent(QPaintEvent *)
{
    QPixmap pixmap(m_currentUrl);
    m_pixmap = pixmap;
    QPainter painter(this);
    if (m_isEmptyBtn == false) {
        if (!m_isHistorybtn) {
            int mode = m_settings->getOption("mode").toInt();
            if (mode == 0)
                setFixedSize(80, 58);
        }
        QRectF frameRect = this->rect();
        QRectF rect(frameRect.left() + 2, frameRect.top() + 2, frameRect.width() - 4, frameRect.height() - 4);
        QRectF hover(frameRect.left() + 3, frameRect.top() + 3, frameRect.width() - 6, frameRect.height() - 6);
        if (m_isHistorybtn)
            rect = hover = frameRect;
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        //m_pixmap = m_pixmap.scaled(m_pixmap.size() * devicePixelRatioF());
        QColor actcolor = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color();//活动色
        QColor pressBrush, focus, hoverFrame, base, hoverbrush;
        int type = DGuiApplicationHelper::instance()->paletteType();
        if (type == 0)
            type = DGuiApplicationHelper::instance()->themeType();
        if (type == 1) {
            pressBrush = QColor(0, 0, 0);
            pressBrush.setAlphaF(0.1);
            focus = actcolor;
            hoverFrame = actcolor;
            hoverFrame.setAlphaF(0.2);
            if (m_page) {
                base = QColor(0, 0, 0);
                base.setAlphaF(0.04);
                hoverbrush = QColor(0, 0, 0);
                hoverbrush.setAlphaF(0.04);
            } else {
                base = Qt::white;
                hoverbrush = Qt::white;
            }
        } else {
            pressBrush = QColor(0, 0, 0);
            pressBrush.setAlphaF(0.5);
            focus = actcolor;
            hoverFrame = actcolor;
            hoverFrame.setAlphaF(0.2);
            base = QColor(48, 48, 48);
            hoverbrush = QColor(255, 255, 255);
            hoverbrush.setAlphaF(0.1);
        }
        if (m_isHistorybtn) {
            if (type == 1) {
                pressBrush = QColor(0, 0, 0);
                pressBrush.setAlphaF(0.1);
                focus = actcolor;
                base = Qt::transparent;
                hoverbrush = QColor(255, 255, 255);
                hoverbrush.setAlphaF(0.6);
            } else {
                pressBrush = QColor(0, 0, 0);
                pressBrush.setAlphaF(0.5);
                focus = actcolor;
                base = QColor("#252525");
                hoverbrush = QColor(255, 255, 255);
                hoverbrush.setAlphaF(0.1);
            }
            if (hasFocus()) {
                if (m_isPress) {
                    painter.setBrush(QBrush(pressBrush));
                    QPen pen;
                    pen.setColor(pressBrush);
                    painter.setPen(pen);
                    painter.drawRect(rect);
                } else {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QBrush(base));
                    painter.drawRect(rect);
                    QPen pen;
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(Qt::NoBrush);
                    painter.drawRect(rect);
                }
            } else {
                if (m_isHover) {
                    QPen pen;
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QBrush(hoverbrush));
                    painter.drawRect(rect);
                } else if (m_isPress) {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QBrush(pressBrush));
                    painter.drawRect(rect);
                } else {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QBrush(base));
                    painter.drawRect(rect);
                }
            }
        } else {
            if (hasFocus()) {
                if (m_isPress) {
                    painter.setBrush(QBrush(pressBrush));
                    QPen pen;
                    pen.setColor(pressBrush);
                    painter.setPen(pen);
                    painter.drawRoundRect(rect, 25, 30);
                } else {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QBrush(base));
                    painter.drawRoundRect(rect, 25, 30);
                    QPen pen;
                    pen.setColor(focus);
                    pen.setWidth(2);
                    painter.setPen(pen);
                    painter.setBrush(Qt::NoBrush);
                    painter.drawRoundRect(rect, 25, 30);
                }
            } else {
                if (m_isHover) {
                    QPen pen;
                    pen.setColor(hoverFrame);
                    pen.setWidth(1);
                    painter.setPen(pen);
                    painter.setBrush(QBrush(hoverbrush));
                    painter.drawRoundRect(rect, 25, 30);
                } else if (m_isPress) {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QBrush(pressBrush));
                    painter.drawRoundRect(rect, 25, 30);
                } else {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QBrush(base));
                    painter.drawRoundRect(rect, 25, 30);
                }
            }
        }
    }
    drawCenterPixMap(painter);
}

/**
 * @brief SetAttrRecur
 * @param elem
 * @param strtagname XML元素标签名
 * @param strattr setAttribute(name)
 * @param strattrval setAttribute(value)
 * 修改加载的svg图片的颜色
 */
void IconButton::SetAttrRecur(QDomElement elem, QString strtagname, QString strattr, QString strattrval)
{
    if (m_mode != 1 && m_mode != 3 && !m_isHistorybtn && m_mode != 5) {
        if (elem.tagName().compare(strtagname) == 0 && elem.attribute(strattr) != "none" && elem.attribute(strattr) != "") {
            elem.setAttribute(strattr, strattrval);
            if (m_buttonStatus == 0)
                elem.setAttribute("fill-opacity", 0.75);
            if (m_buttonStatus == 1)
                elem.setAttribute("fill-opacity", 0.65);
            if (m_buttonStatus == 2)
                elem.setAttribute("fill-opacity", 1);
        }
        if (m_mode == 0) {
            elem.setAttribute(strattr, strattrval);
            if (m_buttonStatus == 0)
                elem.setAttribute("fill-opacity", 0.75);
            if (m_buttonStatus == 1)
                elem.setAttribute("fill-opacity", 0.65);
            if (m_buttonStatus == 2)
                elem.setAttribute("fill-opacity", 1);
        }
        if (m_isEmptyBtn == true || (m_mode == 4 && m_themetype == 1)) {
            strtagname = "path";
            if (elem.tagName().compare(strtagname) == 0 && elem.attribute(strattr) != "none" && elem.attribute(strattr) != "") {
                if (m_buttonStatus == 2) {
                    elem.setAttribute(strattr, strattrval);
                }
            }
        }
        if (m_mode == 4 && m_themetype == 2) {
            strtagname = "text";
            if (elem.tagName().compare(strtagname) == 0 && elem.attribute(strattr) != "none" && elem.attribute(strattr) != "") {
                elem.setAttribute(strattr, strattrval);
            }
        }
        if (m_mode == 6) {
            strtagname = "text";
            if (elem.tagName().compare(strtagname) == 0 && elem.attribute(strattr) != "none" && elem.attribute(strattr) != "") {
                elem.setAttribute(strattr, strattrval);
            }
        }
        for (int i = 0; i < elem.childNodes().count(); i++) {
            if (!elem.childNodes().at(i).isElement()) {
                continue;
            }
            SetAttrRecur(elem.childNodes().at(i).toElement(), strtagname, strattr, strattrval);
        }
    }
}

void IconButton::drawCenterPixMap(QPainter &painter)
{
    painter.save();

    QFile file(m_currentUrl);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();

    QDomDocument doc;
    doc.setContent(baData);

    file.close();

    SetAttrRecur(doc.documentElement(), "g", "fill", Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color().name());
    QRectF frameRect = this->rect();
    QRectF rect(frameRect.left() + 2, frameRect.top() + 2, frameRect.width() - 4, frameRect.height() - 4);
    QRectF pixRect = m_pixmap.rect();
    pixRect.moveCenter(rect.center());
//    m_iconRenderer = new DSvgRenderer(doc.toByteArray(), this);
    m_iconRenderer->load(doc.toByteArray());
    m_iconRenderer->render(&painter, pixRect);
    painter.restore();
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
