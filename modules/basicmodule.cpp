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

#include "basicmodule.h"
#include "dthememanager.h"
#include "utils.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>

DWIDGET_USE_NAMESPACE

BasicModule::BasicModule(QWidget *parent)
    : QWidget(parent),
      m_expressionBar(new ExpressionBar),
      m_simpleKeypad(new SimpleKeypad),
      m_scientificKeypad(new ScientificKeypad),
      m_keypadLayout(new QStackedLayout)
{
    m_keypadLayout->addWidget(m_simpleKeypad);
    m_keypadLayout->addWidget(m_scientificKeypad);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_expressionBar);
    layout->addSpacing(1);
    layout->addLayout(m_keypadLayout);
    layout->setSpacing(0);
    layout->setMargin(0);

    setMouseTracking(true);
    initTheme();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &BasicModule::initTheme);
    connect(m_expressionBar, &ExpressionBar::keyPress, this, &BasicModule::handleEditKeyPress);
    connect(m_expressionBar, &ExpressionBar::clearStateChanged, this, &BasicModule::handleClearStateChanged);
    connect(m_simpleKeypad, &SimpleKeypad::buttonPressed, this, &BasicModule::handleKeypadButtonPress);
}

BasicModule::~BasicModule()
{
}

void BasicModule::switchToSimpleKeypad()
{
    m_keypadLayout->setCurrentIndex(0);
}

void BasicModule::switchToScientificKeypad()
{
    m_keypadLayout->setCurrentIndex(1);
}

void BasicModule::initTheme()
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
}

void BasicModule::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;
    const QString keyText = e->text();

    switch (e->key()) {
    case Qt::Key_0:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_0);
        break;
    case Qt::Key_1:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_1);
        break;
    case Qt::Key_2:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_2);
        break;
    case Qt::Key_3:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_3);
        break;
    case Qt::Key_4:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_4);
        break;
    case Qt::Key_5:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_5);
        break;
    case Qt::Key_6:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_6);
        break;
    case Qt::Key_7:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_7);
        break;
    case Qt::Key_8:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_8);
        break;
    case Qt::Key_9:
        m_expressionBar->enterNumberEvent("");
        m_simpleKeypad->animate(SimpleKeypad::Key_9);
        break;
    case Qt::Key_Plus:
        m_simpleKeypad->animate(SimpleKeypad::Key_Plus);
        break;
    case Qt::Key_Minus: case Qt::Key_Underscore:
        m_simpleKeypad->animate(SimpleKeypad::Key_Min);
        break;
    case Qt::Key_Asterisk: case Qt::Key_X:
        m_simpleKeypad->animate(SimpleKeypad::Key_Mult);
        break;
    case Qt::Key_Slash:
        m_simpleKeypad->animate(SimpleKeypad::Key_Div);
        break;
    case Qt::Key_Enter: case Qt::Key_Return: case Qt::Key_Equal:
        (static_cast<QPushButton *>(m_simpleKeypad->button(SimpleKeypad::Key_Equals)))->animateClick();
        break;
    case Qt::Key_Backspace:
        m_simpleKeypad->animate(SimpleKeypad::Key_Backspace);
        break;
    case Qt::Key_Period:
        (static_cast<QPushButton *>(m_simpleKeypad->button(SimpleKeypad::Key_Point)))->animateClick();
        break;
    case Qt::Key_Escape:
        (static_cast<QPushButton *>(m_simpleKeypad->button(SimpleKeypad::Key_Clear)))->animateClick();
        break;
    case Qt::Key_ParenLeft: case Qt::Key_ParenRight:
        m_expressionBar->setContinue(true);
        m_simpleKeypad->animate(SimpleKeypad::Key_Brackets);
        break;
    case Qt::Key_Percent:
        m_simpleKeypad->animate(SimpleKeypad::Key_Percent);
        break;
    }
}

void BasicModule::handleKeypadButtonPress(int key)
{
    switch (key) {
    case SimpleKeypad::Key_0:            m_expressionBar->enterNumberEvent("0");     break;
    case SimpleKeypad::Key_1:            m_expressionBar->enterNumberEvent("1");     break;
    case SimpleKeypad::Key_2:            m_expressionBar->enterNumberEvent("2");     break;
    case SimpleKeypad::Key_3:            m_expressionBar->enterNumberEvent("3");     break;
    case SimpleKeypad::Key_4:            m_expressionBar->enterNumberEvent("4");     break;
    case SimpleKeypad::Key_5:            m_expressionBar->enterNumberEvent("5");     break;
    case SimpleKeypad::Key_6:            m_expressionBar->enterNumberEvent("6");     break;
    case SimpleKeypad::Key_7:            m_expressionBar->enterNumberEvent("7");     break;
    case SimpleKeypad::Key_8:            m_expressionBar->enterNumberEvent("8");     break;
    case SimpleKeypad::Key_9:            m_expressionBar->enterNumberEvent("9");     break;
    case SimpleKeypad::Key_Plus:         m_expressionBar->enterSymbolEvent("+");     break;
    case SimpleKeypad::Key_Min:          m_expressionBar->enterSymbolEvent("-");     break;
    case SimpleKeypad::Key_Mult:         m_expressionBar->enterSymbolEvent("*");     break;
    case SimpleKeypad::Key_Div:          m_expressionBar->enterSymbolEvent("/");     break;
    case SimpleKeypad::Key_Percent:      m_expressionBar->enterSymbolEvent("%");     break;
    case SimpleKeypad::Key_Equals:       m_expressionBar->enterEqualEvent();         break;
    case SimpleKeypad::Key_Clear:        m_expressionBar->enterClearEvent();         break;
    case SimpleKeypad::Key_Backspace:    m_expressionBar->enterBackspaceEvent();     break;
    case SimpleKeypad::Key_Point:        m_expressionBar->enterPointEvent();         break;
    case SimpleKeypad::Key_Brackets:     m_expressionBar->enterBracketsEvent();      break;
    }
}

void BasicModule::handleClearStateChanged(bool isAllClear)
{
    TextButton *btn = static_cast<TextButton *>(m_simpleKeypad->button(SimpleKeypad::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

void BasicModule::paintEvent(QPaintEvent *)
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
