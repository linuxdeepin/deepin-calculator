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

BasicModule::BasicModule(QWidget *parent)
    : DWidget(parent),
      m_expressionBar(new ExpressionBar),
      m_basicKeypad(new BasicKeypad),
      m_keypadLayout(new QStackedLayout)
{
    m_keypadLayout->addWidget(m_basicKeypad);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_expressionBar);
    layout->addLayout(m_keypadLayout);

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,11);

    setMouseTracking(true);
    setFocus();
    //initTheme();
    m_expressionBar->initTheme(DGuiApplicationHelper::instance()->themeType());
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,m_expressionBar,&ExpressionBar::initTheme);

    DPalette pal = this->palette();
    pal.setColor(DPalette::Light,QColor(248,248,248));
    this->setPalette(pal);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged, this, &BasicModule::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, m_basicKeypad, &BasicKeypad::buttonThemeChanged);
    connect(m_expressionBar, &ExpressionBar::keyPress, this, &BasicModule::handleEditKeyPress);
    connect(m_expressionBar, &ExpressionBar::clearStateChanged, this, &BasicModule::handleClearStateChanged);
    connect(m_basicKeypad, &BasicKeypad::buttonPressed, this, &BasicModule::handleKeypadButtonPress);
    connect(m_basicKeypad, &BasicKeypad::equalPressed, this, &BasicModule::equalButtonPress);
    connect(m_basicKeypad, &BasicKeypad::moveLeft, [=] {m_expressionBar->moveLeft();});
    connect(m_basicKeypad, &BasicKeypad::moveRight, [=] {m_expressionBar->moveRight();});

    //m_expBarColor = "#F8F8F8";
    //m_expBarSepColor = "#F8F8F8";
}

BasicModule::~BasicModule()
{
}

void BasicModule::switchToSimpleKeypad()
{
}

void BasicModule::switchToScientificKeypad()
{
}

void BasicModule::initTheme(int type)
{
    /*DPalette pal;
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        m_expBarColor = "#F8F8F8";
        m_expBarSepColor = "#F8F8F8";
        pal = this->palette();
        pal.setColor(DPalette::Light,QColor("#F8F8F8"));
        this->setPalette(pal);
        m_basicKeypad->buttonThemeChanged(type);
    }
    else {
        m_expBarColor = "#252525";
        m_expBarSepColor = "#252525";
        pal = this->palette();
        pal.setColor(DPalette::Dark,QColor(17,17,17));
        this->setPalette(pal);
        m_basicKeypad->buttonThemeChanged(type);
    }*/
    m_expressionBar->initTheme(type);
    /*switch (type) {
    case 0: case 1:
        m_expBarColor = "#F8F8F8";
        m_expBarSepColor = "#F8F8F8";
        m_btnSepColor = QColor("#F8F8F8");
        pal = this->palette();
        pal.setColor(DPalette::Light,QColor("#F8F8F8"));
        this->setPalette(pal);
        m_basicKeypad->buttonThemeChanged(type);
        break;
    case 2:
        m_expBarColor = "#111111";
        m_expBarSepColor = "#303030";
        m_btnSepColor = QColor("#2D2D2D");
        pal = this->palette();
        pal.setColor(DPalette::Dark,QColor(17,17,17));
        this->setPalette(pal);
        m_basicKeypad->buttonThemeChanged(type);
    default:
        break;
    }*/
    update();
}

void BasicModule::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;
    const QString keyText = e->text();
    bool lineFocus = m_expressionBar->getInputEdit()->hasFocus();
    //m_expressionBar->clearSelection();

    switch (e->key()) {
    case Qt::Key_0:
        m_expressionBar->enterNumberEvent("0");
        m_basicKeypad->animate(BasicKeypad::Key_0);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_1:
        m_expressionBar->enterNumberEvent("1");
        m_basicKeypad->animate(BasicKeypad::Key_1);
        //m_basicKeypad->button(BasicKeypad::Key_1)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_2:
        m_expressionBar->enterNumberEvent("2");
        m_basicKeypad->animate(BasicKeypad::Key_2);
        //m_basicKeypad->button(BasicKeypad::Key_2)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_3:
        m_expressionBar->enterNumberEvent("3");
        m_basicKeypad->animate(BasicKeypad::Key_3);
        //m_basicKeypad->button(BasicKeypad::Key_3)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_4:
        m_expressionBar->enterNumberEvent("4");
        m_basicKeypad->animate(BasicKeypad::Key_4);
        //m_basicKeypad->button(BasicKeypad::Key_4)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_5:
        m_expressionBar->enterNumberEvent("5");
        m_basicKeypad->animate(BasicKeypad::Key_5);
        //m_basicKeypad->button(BasicKeypad::Key_5)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_6:
        m_expressionBar->enterNumberEvent("6");
        m_basicKeypad->animate(BasicKeypad::Key_6);
        //m_basicKeypad->button(BasicKeypad::Key_6)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_7:
        m_expressionBar->enterNumberEvent("7");
        m_basicKeypad->animate(BasicKeypad::Key_7);
        //m_basicKeypad->button(BasicKeypad::Key_7)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_8:
        m_expressionBar->enterNumberEvent("8");
        m_basicKeypad->animate(BasicKeypad::Key_8);
        //m_basicKeypad->button(BasicKeypad::Key_8)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_9:
        m_expressionBar->enterNumberEvent("9");
        m_basicKeypad->animate(BasicKeypad::Key_9);
        //m_basicKeypad->button(BasicKeypad::Key_9)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Plus:
        m_expressionBar->enterSymbolEvent("+");
        m_basicKeypad->animate(BasicKeypad::Key_Plus);
        //m_basicKeypad->button(BasicKeypad::Key_Plus)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Minus: case Qt::Key_Underscore:
        m_expressionBar->enterSymbolEvent("-");
        m_basicKeypad->animate(BasicKeypad::Key_Min);
        //m_basicKeypad->button(BasicKeypad::Key_Min)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Asterisk:
        m_expressionBar->enterSymbolEvent("*");
        m_basicKeypad->animate(BasicKeypad::Key_Mult);
        //m_basicKeypad->button(BasicKeypad::Key_Mult)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Slash:
        m_expressionBar->enterSymbolEvent("/");
        m_basicKeypad->animate(BasicKeypad::Key_Div);
        //m_basicKeypad->button(BasicKeypad::Key_Div)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Enter: case Qt::Key_Return: case Qt::Key_Equal:
        //(dynamic_cast<DPushButton *>(m_basicKeypad->button(BasicKeypad::Key_Equals)))->animateClick();
        //m_basicKeypad->button()->animateClick();
        m_basicKeypad->animate(BasicKeypad::Key_Equals);
        m_expressionBar->settingLinkage();
        m_expressionBar->addUndo();
        setFocus();
        break;
    case Qt::Key_Backspace:
        m_expressionBar->enterBackspaceEvent();
        m_basicKeypad->animate(BasicKeypad::Key_Backspace);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Period:
        m_basicKeypad->animate(BasicKeypad::Key_Point);
        m_expressionBar->enterPointEvent();
        //(static_cast<DPushButton *>(m_basicKeypad->button(BasicKeypad::Key_Point)))->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Escape:
        (static_cast<DPushButton *>(m_basicKeypad->button(BasicKeypad::Key_Clear)))->animateClick();
        //m_expressionBar->enterClearEvent();
        //m_basicKeypad->animate(BasicKeypad::Key_Clear);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_ParenLeft:
        //m_expressionBar->setContinue(true);
        //m_basicKeypad->animate(BasicKeypad::Key_Brackets);
        //m_expressionBar->enterBracketsEvent();
        //m_basicKeypad->button(BasicKeypad::Key_Brackets)->animateClick();
        m_expressionBar->enterLeftBracketsEvent();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_ParenRight:
        m_expressionBar->enterRightBracketsEvent();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Percent:
        m_expressionBar->enterPercentEvent();
        m_basicKeypad->animate(BasicKeypad::Key_Percent);
        //m_basicKeypad->button(BasicKeypad::Key_Percent)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_C:
        if (isPressCtrl) {
            m_expressionBar->copyResultToClipboard();
            setFocus();
        }
        break;
    case Qt::Key_V:
        if (isPressCtrl) {
            m_expressionBar->copyClipboard2Result();
            m_expressionBar->addUndo();
        }
        break;
    case Qt::Key_A:
        if (isPressCtrl) {
            m_expressionBar->allElection();
        }
        break;
    case Qt::Key_X:
        if (isPressCtrl) {
            m_expressionBar->shear();
        } else {
            m_expressionBar->enterSymbolEvent("*");
            m_basicKeypad->animate(BasicKeypad::Key_Mult);
            m_expressionBar->addUndo();
        }
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Delete:
        m_expressionBar->enterClearEvent();
        //m_expressionBar->enterDeleteEvent();
        //m_expressionBar->addUndo();
        break;
    case Qt::Key_Left:
        m_expressionBar->moveLeft();
        break;
    case Qt::Key_Right:
        m_expressionBar->moveRight();
        break;
    case Qt::Key_Z:
        if (isPressCtrl)
            m_expressionBar->Undo();
        break;
    case Qt::Key_Y:
        if (isPressCtrl)
            m_expressionBar->Redo();
        break;
    case Qt::Key_E:
        m_expressionBar->entereEvent();
        m_expressionBar->addUndo();
    default:
        break;
    }
    //m_expressionBar->setSelection();
    if (lineFocus)
        m_expressionBar->getInputEdit()->setFocus();
    else
        setFocus();
}

void BasicModule::handleKeypadButtonPress(int key)
{
    m_expressionBar->clearSelection();
    switch (key) {
    case BasicKeypad::Key_0:            m_expressionBar->enterNumberEvent("0");     break;
    case BasicKeypad::Key_1:            m_expressionBar->enterNumberEvent("1");     break;
    case BasicKeypad::Key_2:            m_expressionBar->enterNumberEvent("2");     break;
    case BasicKeypad::Key_3:            m_expressionBar->enterNumberEvent("3");     break;
    case BasicKeypad::Key_4:            m_expressionBar->enterNumberEvent("4");     break;
    case BasicKeypad::Key_5:            m_expressionBar->enterNumberEvent("5");     break;
    case BasicKeypad::Key_6:            m_expressionBar->enterNumberEvent("6");     break;
    case BasicKeypad::Key_7:            m_expressionBar->enterNumberEvent("7");     break;
    case BasicKeypad::Key_8:            m_expressionBar->enterNumberEvent("8");     break;
    case BasicKeypad::Key_9:            m_expressionBar->enterNumberEvent("9");     break;
    case BasicKeypad::Key_Plus:         m_expressionBar->enterSymbolEvent("+");     break;
    case BasicKeypad::Key_Min:          m_expressionBar->enterSymbolEvent("-");     break;
    case BasicKeypad::Key_Mult:         m_expressionBar->enterSymbolEvent("*");     break;
    case BasicKeypad::Key_Div:          m_expressionBar->enterSymbolEvent("/");     break;
    case BasicKeypad::Key_Percent:      m_expressionBar->enterPercentEvent();       break;
    case BasicKeypad::Key_Equals:       m_expressionBar->settingLinkage();          break;
    case BasicKeypad::Key_Clear:        m_expressionBar->enterClearEvent();         break;
    case BasicKeypad::Key_Backspace:    m_expressionBar->enterBackspaceEvent();     break;
    case BasicKeypad::Key_Point:        m_expressionBar->enterPointEvent();         break;
    case BasicKeypad::Key_Brackets:     m_expressionBar->enterBracketsEvent();      break;
    }
    m_expressionBar->addUndo();
}

void BasicModule::equalButtonPress()
{
    m_expressionBar->settingLinkage();
    m_expressionBar->addUndo();
}

void BasicModule::handleClearStateChanged(bool isAllClear)
{
    TextButton *btn = static_cast<TextButton *>(m_basicKeypad->button(BasicKeypad::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

void BasicModule::paintEvent(QPaintEvent *)
{
    /*QPainter painter(this);
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
                           rect().height()));*/
}

void BasicModule::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}
