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

#include "simplemodule.h"
#include "dthememanager.h"
#include "utils.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>

DWIDGET_USE_NAMESPACE

SimpleModule::SimpleModule(QWidget *parent)
    : QWidget(parent),
      m_expressionBar(new ExpressionBar),
      m_clearBtn(new TextButton("C")),
      m_percentBtn(new TextButton("%")),
      m_backBtn(new IconButton),
      m_divBtn(new TextButton("÷")),
      m_num7Btn(new TextButton("7")),
      m_num8Btn(new TextButton("8")),
      m_num9Btn(new TextButton("9")),
      m_multBtn(new TextButton("×")),
      m_num4Btn(new TextButton("4")),
      m_num5Btn(new TextButton("5")),
      m_num6Btn(new TextButton("6")),
      m_minBtn(new TextButton("－")),
      m_num1Btn(new TextButton("1")),
      m_num2Btn(new TextButton("2")),
      m_num3Btn(new TextButton("3")),
      m_plusBtn(new TextButton("＋")),
      m_num0Btn(new TextButton("0")),
      m_pointBtn(new TextButton(".")),
      m_bracketsBtn(new TextButton("( )")),
      m_equalBtn(new TextButton("＝"))
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(m_expressionBar, 0, 0, 1, 4);
    layout->addWidget(m_clearBtn, 2, 0);
    layout->addWidget(m_percentBtn, 2, 1);
    layout->addWidget(m_backBtn, 2, 2);
    layout->addWidget(m_divBtn, 2, 3);
    layout->addWidget(m_num7Btn, 3, 0);
    layout->addWidget(m_num8Btn, 3, 1);
    layout->addWidget(m_num9Btn, 3, 2);
    layout->addWidget(m_multBtn, 3, 3);
    layout->addWidget(m_num4Btn, 4, 0);
    layout->addWidget(m_num5Btn, 4, 1);
    layout->addWidget(m_num6Btn, 4, 2);
    layout->addWidget(m_minBtn, 4, 3);
    layout->addWidget(m_num1Btn, 5, 0);
    layout->addWidget(m_num2Btn, 5, 1);
    layout->addWidget(m_num3Btn, 5, 2);
    layout->addWidget(m_plusBtn, 5, 3);
    layout->addWidget(m_num0Btn, 6, 0);
    layout->addWidget(m_pointBtn, 6, 1);
    layout->addWidget(m_bracketsBtn, 6, 2);
    layout->addWidget(m_equalBtn, 6, 3);
    layout->setMargin(0);
    layout->setSpacing(1);

    m_divBtn->setObjectName("SymbolButton");
    m_multBtn->setObjectName("SymbolButton");
    m_minBtn->setObjectName("SymbolButton");
    m_plusBtn->setObjectName("SymbolButton");
    m_equalBtn->setObjectName("EqualButton");

    setMouseTracking(true);
    initTheme();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &SimpleModule::initTheme);
    connect(m_expressionBar, &ExpressionBar::keyPress, this, &SimpleModule::handleEditKeyPress);
    connect(m_expressionBar, &ExpressionBar::clearStateChanged, this, &SimpleModule::handleClearStateChanged);
    connect(m_num0Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num1Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num2Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num3Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num4Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num5Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num6Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num7Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num8Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_num9Btn, &QPushButton::clicked, this, &SimpleModule::enterNumberEvent);
    connect(m_plusBtn, &QPushButton::clicked, this, &SimpleModule::enterSymbolEvent);
    connect(m_minBtn, &QPushButton::clicked, this, &SimpleModule::enterSymbolEvent);
    connect(m_multBtn, &QPushButton::clicked, this, &SimpleModule::enterSymbolEvent);
    connect(m_divBtn, &QPushButton::clicked, this, &SimpleModule::enterSymbolEvent);
    connect(m_percentBtn, &QPushButton::clicked, this, &SimpleModule::enterSymbolEvent);
    connect(m_bracketsBtn, &QPushButton::clicked, m_expressionBar, &ExpressionBar::enterBracketsEvent);
    connect(m_clearBtn, &QPushButton::clicked, m_expressionBar, &ExpressionBar::enterClearEvent);
    connect(m_backBtn, &QPushButton::clicked, m_expressionBar, &ExpressionBar::enterBackspaceEvent);
    connect(m_equalBtn, &QPushButton::clicked, m_expressionBar, &ExpressionBar::enterEqualEvent);
    connect(m_pointBtn, &QPushButton::clicked, m_expressionBar, &ExpressionBar::enterPointEvent);
}

SimpleModule::~SimpleModule()
{
}

void SimpleModule::initTheme()
{
    const QString theme = DThemeManager::instance()->theme();

    if (theme == "light") {
        m_expBarColor = "#FBFBFB";
        m_expBarSepColor = "#E1E1E1";
        m_btnSepColor = QColor(242, 242, 242);
    } else {
        m_expBarColor = "#111111";
        m_expBarSepColor = "#303030";
        m_btnSepColor = QColor("#2D2D2D");
    }

    m_backBtn->setIcon(QString(":/images/delete_%1_normal.svg").arg(theme));
}

void SimpleModule::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;
    const QString keyText = e->text();

    switch (e->key()) {
    case Qt::Key_0:
        m_expressionBar->enterNumberEvent("");
        m_num0Btn->animate();
        break;
    case Qt::Key_1:
        m_expressionBar->enterNumberEvent("");
        m_num1Btn->animate();
        break;
    case Qt::Key_2:
        m_expressionBar->enterNumberEvent("");
        m_num2Btn->animate();
        break;
    case Qt::Key_3:
        m_expressionBar->enterNumberEvent("");
        m_num3Btn->animate();
        break;
    case Qt::Key_4:
        m_expressionBar->enterNumberEvent("");
        m_num4Btn->animate();
        break;
    case Qt::Key_5:
        m_expressionBar->enterNumberEvent("");
        m_num5Btn->animate();
        break;
    case Qt::Key_6:
        m_expressionBar->enterNumberEvent("");
        m_num6Btn->animate();
        break;
    case Qt::Key_7:
        m_expressionBar->enterNumberEvent("");
        m_num7Btn->animate();
        break;
    case Qt::Key_8:
        m_expressionBar->enterNumberEvent("");
        m_num8Btn->animate();
        break;
    case Qt::Key_9:
        m_expressionBar->enterNumberEvent("");
        m_num9Btn->animate();
        break;
    case Qt::Key_Plus:
        m_plusBtn->animate();
        break;
    case Qt::Key_Minus: case Qt::Key_Underscore:
        m_minBtn->animate();
        break;
    case Qt::Key_Asterisk: case Qt::Key_X:
        m_multBtn->animate();
        break;
    case Qt::Key_Slash:
        m_divBtn->animate();
        break;
    case Qt::Key_Enter: case Qt::Key_Return: case Qt::Key_Equal:
        m_equalBtn->animateClick();
        break;
    case Qt::Key_Backspace:
        m_backBtn->animate();
        break;
    case Qt::Key_Period:
        m_pointBtn->animateClick();
        break;
    case Qt::Key_Escape:
        m_clearBtn->animateClick();
        break;
    case Qt::Key_ParenLeft: case Qt::Key_ParenRight:
        m_expressionBar->setContinue(true);
        m_bracketsBtn->animate();
        break;
    case Qt::Key_Percent:
        m_percentBtn->animate();
        break;
    }
}

void SimpleModule::handleClearStateChanged(bool isAllClear)
{
    if (isAllClear) {
        m_clearBtn->setText("AC");
    } else {
        m_clearBtn->setText("C");
    }
}

void SimpleModule::enterNumberEvent()
{
    QPushButton *btn = static_cast<QPushButton *>(sender());
    m_expressionBar->enterNumberEvent(btn->text());
}

void SimpleModule::enterSymbolEvent()
{
    QPushButton *btn = static_cast<QPushButton *>(sender());
    m_expressionBar->enterSymbolEvent(btn->text());
}

void SimpleModule::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);

    // draw expression bar background;
    painter.setBrush(m_expBarColor);
    painter.drawRect(QRect(0, 0, rect().width(), m_expressionBar->height() - 1));

    // draw expression bar bottom separator.
    painter.setBrush(m_expBarSepColor);
    painter.drawRect(QRect(0, m_expressionBar->height(), rect().width(), 1));

    // draw buttons separator.
    painter.setBrush(m_btnSepColor);
    painter.drawRect(QRect(rect().x(),
                           m_expressionBar->height() + 1,
                           rect().width(),
                           rect().height()));
}
