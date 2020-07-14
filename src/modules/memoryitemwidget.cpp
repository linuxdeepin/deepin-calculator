/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
*
* Maintainer: xiajing <xiajing@uniontech.com>
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

#include "memoryitemwidget.h"

#include <QPainter>
#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QEvent>
#include <QContextMenuEvent>
#include <QClipboard>
#include <DGuiApplicationHelper>

MemoryItemWidget::MemoryItemWidget(QWidget *parent)
    : QWidget(parent)
    , m_btnplus(new MemoryButton(QString("M+"), true, this))
    , m_btnminus(new MemoryButton(QString("M-"), true, this))
    , m_btnclean(new MemoryButton(QString("MC"), true, this))
    , m_label(new QLabel(this))
{
    QVBoxLayout *layV = new QVBoxLayout();
    QHBoxLayout *layH = new QHBoxLayout(this);
    QHBoxLayout *lay = new QHBoxLayout();

    lay->addStretch();
    lay->setSpacing(0);
    lay->addWidget(m_btnclean);
    lay->addWidget(m_btnplus);
    lay->addWidget(m_btnminus);
//    lay->addSpacing(20);
    layV->setMargin(0);
    layV->addWidget(m_label);
    layV->addStretch();
//    lineedit->setAlignment(Qt::AlignTop);
    QFont font;
    font.setPixelSize(30);
    m_label->setFont(font);
    m_label->setAttribute(Qt::WA_TranslucentBackground);

    m_label->setAlignment(Qt::AlignRight | Qt::AlignTop);
//    lineedit->setAlignment(Qt::AlignCenter | Qt::AlignBottom | Qt::AlignHCenter);
//    lineedit->setTextMargins(0, 0, 10, 6);

    m_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
//    layV->addLayout(laylabel);
    layV->addLayout(lay);
//    layV->setContentsMargins(0, 0, 0, 0);
    layH->addLayout(layV);
    layH->addSpacing(20);
    layH->setContentsMargins(0, 0, 0, 6);
    this->setLayout(layH);
//    this->setContentsMargins(0, 0, 0, 0);

    connect(m_btnplus, &QPushButton::clicked, this, &MemoryItemWidget::plusbtnclicked);
    connect(m_btnminus, &QPushButton::clicked, this, &MemoryItemWidget::minusbtnclicked);
    connect(m_btnclean, &QPushButton::clicked, this, &MemoryItemWidget::cleanbtnclicked);
    m_btnplus->setHidden(true);
    m_btnminus->setHidden(true);
    m_btnclean->setHidden(true);
//    m_font.setPixelSize(18);
}

void MemoryItemWidget::enterEvent(QEvent *event)
{
    m_ishover = true;
    m_btnplus->setHidden(false);
    m_btnminus->setHidden(false);
    m_btnclean->setHidden(false);
    update();
//    this->setAttribute(Qt::WA_StyledBackground, true);
//    this->setStyleSheet("background-color: rgb(245,245,245)");
    QWidget::enterEvent(event);
}

void MemoryItemWidget::leaveEvent(QEvent *event)
{
//    clearFocus();
    m_ishover = false;
    m_btnplus->setHidden(true);
    m_btnminus->setHidden(true);
    m_btnclean->setHidden(true);
    emit itemchanged(m_themetype);
//    this->setAttribute(Qt::WA_StyledBackground, true);
//    this->setStyleSheet("background-color: rgb(255,255,255)");
    QWidget::leaveEvent(event);
}

void MemoryItemWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
    setFocus();
    m_ispress = true;
    DPalette pl1 = this->palette();
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, Qt::black);
        pl1.setColor(DPalette::HighlightedText, Qt::black);
    } else {
        pl1.setColor(DPalette::Text, QColor("#FFFFFF"));
        pl1.setColor(DPalette::HighlightedText, QColor("#FFFFFF"));
    }
    pl1.setColor(DPalette::Button, Qt::transparent);
    pl1.setColor(DPalette::Highlight, Qt::transparent);

    // pl.setColor(DPalette::Text,QColor(48,48,48));

    m_label->setPalette(pl1);
    update();
    QWidget::mousePressEvent(event);
}

void MemoryItemWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
    clearFocus();
    m_ispress = false;
    DPalette pl1 = this->palette();
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, Qt::black);
        pl1.setColor(DPalette::HighlightedText, Qt::black);
    } else {
        pl1.setColor(DPalette::Text, QColor("#B4B4B4"));
        pl1.setColor(DPalette::HighlightedText, QColor("#B4B4B4"));
    }
    pl1.setColor(DPalette::Button, Qt::transparent);
    pl1.setColor(DPalette::Highlight, Qt::transparent);

    // pl.setColor(DPalette::Text,QColor(48,48,48));

    m_label->setPalette(pl1);
    QWidget::mouseReleaseEvent(event);
}

void MemoryItemWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QAction *copy = new QAction(tr("Copy"), menu);
    QAction *clean = new QAction(tr("Clear memory item"), menu);
    QAction *plus = new QAction(tr("Add to memory item"), menu);
    QAction *minus = new QAction(tr("Subtract from memory item"), menu);
    menu->addAction(copy);
    menu->addAction(clean);
    menu->addAction(plus);
    menu->addAction(minus);
    if (m_isExpressionEmpty) {
        plus->setEnabled(false);
        minus->setEnabled(false);
    } else {
        plus->setEnabled(true);
        minus->setEnabled(true);
    }
    connect(copy, &QAction::triggered, this, [ = ]() {
        emit menucopy();
    });
    connect(clean, &QAction::triggered, this, [ = ]() {
        emit menuclean();
    });
    connect(plus, &QAction::triggered, this, [ = ]() {
        emit menuplus();
    });
    connect(minus, &QAction::triggered, this, [ = ]() {
        emit menuminus();
    });
    menu->exec(event->globalPos());
    delete menu;
}

void MemoryItemWidget::setTextLabel(QString s)
{
    m_label->setText(s);
}

QString MemoryItemWidget::textLabel()
{
    return m_label->text();
}

void MemoryItemWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QRectF rect = this->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    QColor press, hover;
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        press = QColor(0, 0, 0);
        press.setAlphaF(0.2);
        hover = QColor(0, 0, 0);
        hover.setAlphaF(0.05);
    } else {
        press = QColor(255, 255, 255);
        press.setAlphaF(0.2);
        hover = QColor(255, 255, 255);
        hover.setAlphaF(0.05);
    }
    if (m_ishover) {
        painter.setPen(Qt::NoPen);
//        painter.setFont(m_font);
        painter.setBrush(hover);
        painter.drawRect(rect);
        if (m_ispress) {
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(Qt::NoPen);
            painter.setBrush(press);
            painter.drawRect(rect);

        }
    }
}

void MemoryItemWidget::setLineHight(int line)
{
    m_label->setFixedHeight(45 * line);
}

void MemoryItemWidget::themetypechanged(int type)
{
    m_themetype = type;
    DPalette pl1 = this->palette();
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, Qt::black);
        pl1.setColor(DPalette::HighlightedText, Qt::black);
    } else {
        pl1.setColor(DPalette::Text, QColor("#B4B4B4"));
        pl1.setColor(DPalette::HighlightedText, QColor("#B4B4B4"));
    }
    pl1.setColor(DPalette::Button, Qt::transparent);
    pl1.setColor(DPalette::Highlight, Qt::transparent);

    // pl.setColor(DPalette::Text,QColor(48,48,48));

    m_label->setPalette(pl1);
}

void MemoryItemWidget::isexpressionempty(bool b)
{
    m_isExpressionEmpty = b;
    if (m_isExpressionEmpty) {
        m_btnplus->setEnabled(false);
        m_btnminus->setEnabled(false);
    } else {
        m_btnplus->setEnabled(true);
        m_btnminus->setEnabled(true);
    }
}

MemoryItemWidget::~MemoryItemWidget()
{

}
