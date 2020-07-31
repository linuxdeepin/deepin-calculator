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

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "dthememanager.h"
#include "src/utils.h"
#include "src/math/quantity.h"

const int new_Func_Percent = 0; //0-旧的百分号方法 1-新

BasicModule::BasicModule(QWidget *parent)
    : DWidget(parent)
{
    m_keypadLayout = new QStackedLayout;
    m_basicKeypad = new BasicKeypad;
    m_memoryKeypad = new MemoryKeypad;
    m_insidewidget = false;
    m_memCalbtn = false;
    m_memRCbtn = false;
    m_isallgray = false;
    m_memoryPublic = MemoryPublic::instance(this);
    m_memorylistwidget = m_memoryPublic->getwidget(MemoryPublic::standardleft);
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_expressionBar = new ExpressionBar;
    layout->addWidget(m_expressionBar);
    layout->addWidget(m_memoryKeypad);
    layout->addSpacing(1); //按钮边框多1pix
    layout->addLayout(m_keypadLayout);
    m_keypadLayout->addWidget(m_basicKeypad);
    m_keypadLayout->addWidget(m_memorylistwidget);

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setMouseTracking(true);
    setFocus();

    m_expressionBar->initTheme(DGuiApplicationHelper::instance()->themeType());
    //信号槽连接
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_expressionBar, &ExpressionBar::initTheme);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged, this,
            &BasicModule::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_basicKeypad, &BasicKeypad::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_memoryPublic, &MemoryPublic::setThemeType);
    connect(m_expressionBar, &ExpressionBar::keyPress, this, &BasicModule::handleEditKeyPress);
    connect(m_expressionBar, &ExpressionBar::clearStateChanged, this,
            &BasicModule::handleClearStateChanged);
    connect(m_basicKeypad, &BasicKeypad::buttonPressed, this,
            &BasicModule::handleKeypadButtonPress);
    connect(m_basicKeypad, &BasicKeypad::equalPressed, this, &BasicModule::equalButtonPress);
    connect(m_basicKeypad, &BasicKeypad::moveLeft, [ = ] { m_expressionBar->moveLeft(); });
    connect(m_basicKeypad, &BasicKeypad::moveRight, [ = ] { m_expressionBar->moveRight(); });
    connect(m_memoryKeypad, &MemoryKeypad::moveLeft, [ = ] { m_expressionBar->moveLeft(); });
    connect(m_memoryKeypad, &MemoryKeypad::moveRight, [ = ] { m_expressionBar->moveRight(); });
    connect(m_memoryKeypad, &MemoryKeypad::buttonPressed, this,
            &BasicModule::handleKeypadButtonPress);
    connect(m_memorylistwidget, &MemoryWidget::widgetplus, this, [ = ](int row) {
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetplus(row, m_expressionBar->getInputEdit()->getMemoryAnswer().second);
    });
    connect(m_memorylistwidget, &MemoryWidget::widgetminus, this, [ = ](int row) {
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetminus(row, m_expressionBar->getInputEdit()->getMemoryAnswer().second);
    });
    connect(m_memorylistwidget, &MemoryWidget::insidewidget, this, [ = ]() {
        m_insidewidget = true;
    });
    connect(m_memorylistwidget, &MemoryWidget::mListUnavailable, this, &BasicModule::mUnAvailableEvent);
    connect(m_memorylistwidget, &MemoryWidget::mListAvailable, this, &BasicModule::mAvailableEvent);
    connect(m_memorylistwidget, &MemoryWidget::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
        QString str = p.first;
        m_expressionBar->getInputEdit()->setAnswer(str.remove("\n"), p.second);
        m_expressionBar->getInputEdit()->setFocus();
        if (m_keypadLayout->currentIndex() == 1) {
            m_keypadLayout->setCurrentIndex(0);
            m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
            btn->setEnabled(true);
            btn->setbuttongray(false);
            MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
            btn1->setEnabled(true);
            btn1->setbuttongray(false);
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
            btn2->setbuttongray(false);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
            btn3->setbuttongray(false);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
            btn4->setbuttongray(false);
            MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
            btn5->setbtnlight(false);
            m_memRCbtn = true;
            m_memCalbtn = true;
            m_isallgray = false;
        }
    });
    connect(m_expressionBar->getInputEdit(), &InputEdit::emptyExpression, this, [ = ](bool b) {
        if (b == false) {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
            btn2->setEnabled(true);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
            btn3->setEnabled(true);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
            btn4->setEnabled(true);
            m_memCalbtn = true;
            m_memorylistwidget->expressionempty(b);
        } else {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
            btn3->setEnabled(false);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
            btn4->setEnabled(false);
            m_memCalbtn = false;
            m_memorylistwidget->expressionempty(b);
        }
    });
    //获取科学模式内存是否为空，处理分开初始化科学模式下增加内存切到标准模式Mlist不能点击情况
    if (!m_memoryPublic->isWidgetEmpty(0))
        mAvailableEvent();
    else
        mUnAvailableEvent();
}

BasicModule::~BasicModule() {}

void BasicModule::initTheme(int type)
{
    m_expressionBar->initTheme(type);
    update();
}

/**
 * @brief 物理键盘press事件
 */
void BasicModule::handleEditKeyPress(QKeyEvent *e)
{
    if (m_keypadLayout->currentIndex() == 1)
        return;
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;
    const QString keyText = e->text();

    switch (e->key()) {
    case Qt::Key_0:
        m_expressionBar->enterNumberEvent("0"); //进入按键0事件
        m_basicKeypad->animate(BasicKeypad::Key_0); //按键0动画效果
        m_expressionBar->addUndo(); //动作添加进撤销
        break;
    case Qt::Key_1:
        m_expressionBar->enterNumberEvent("1");
        m_basicKeypad->animate(BasicKeypad::Key_1);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_2:
        m_expressionBar->enterNumberEvent("2");
        m_basicKeypad->animate(BasicKeypad::Key_2);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_3:
        m_expressionBar->enterNumberEvent("3");
        m_basicKeypad->animate(BasicKeypad::Key_3);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_4:
        m_expressionBar->enterNumberEvent("4");
        m_basicKeypad->animate(BasicKeypad::Key_4);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_5:
        m_expressionBar->enterNumberEvent("5");
        m_basicKeypad->animate(BasicKeypad::Key_5);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_6:
        m_expressionBar->enterNumberEvent("6");
        m_basicKeypad->animate(BasicKeypad::Key_6);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_7:
        m_expressionBar->enterNumberEvent("7");
        m_basicKeypad->animate(BasicKeypad::Key_7);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_8:
        m_expressionBar->enterNumberEvent("8");
        m_basicKeypad->animate(BasicKeypad::Key_8);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_9:
        m_expressionBar->enterNumberEvent("9");
        m_basicKeypad->animate(BasicKeypad::Key_9);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Plus:
        m_expressionBar->enterSymbolEvent("+");
        m_basicKeypad->animate(BasicKeypad::Key_Plus);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_expressionBar->enterSymbolEvent("-");
        m_basicKeypad->animate(BasicKeypad::Key_Min);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Asterisk:
        m_expressionBar->enterSymbolEvent("*");
        m_basicKeypad->animate(BasicKeypad::Key_Mult);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Slash:
        m_expressionBar->enterSymbolEvent("/");
        m_basicKeypad->animate(BasicKeypad::Key_Div);
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
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
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Escape:
        m_expressionBar->enterClearEvent();
        m_basicKeypad->animate(BasicKeypad::Key_Clear);
        break;
    case Qt::Key_ParenLeft:
        m_basicKeypad->animate(BasicKeypad::Key_Brackets);
        m_expressionBar->enterLeftBracketsEvent();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_ParenRight:
        m_basicKeypad->animate(BasicKeypad::Key_Brackets);
        m_expressionBar->enterRightBracketsEvent();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Percent:
        if (new_Func_Percent == 0)
            m_expressionBar->enterPercentEvent();
        else
            m_expressionBar->enterPercentEventCommon();
        m_basicKeypad->animate(BasicKeypad::Key_Percent);
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
        m_basicKeypad->animate(BasicKeypad::Key_Clear);
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
    case Qt::Key_L:
        if (isPressCtrl && m_memRCbtn && !m_isallgray) {
            m_memoryKeypad->animate(MemoryKeypad::Key_MC);
            QTimer::singleShot(100, this, [ = ] {
                m_memoryPublic->memoryclean(); //延迟，让动画效果显示
            });
        }
        break;
    case Qt::Key_R:
        if (isPressCtrl && m_memRCbtn && !m_isallgray) {
            m_memoryKeypad->animate(MemoryKeypad::Key_MR);
            m_expressionBar->getInputEdit()->setAnswer(m_memorylistwidget->getfirstnumber().first, m_memorylistwidget->getfirstnumber().second);
        }
        break;
    case Qt::Key_P:
        if (isPressCtrl && m_memCalbtn && !m_isallgray) {
            m_memoryKeypad->animate(MemoryKeypad::Key_Mplus);
            m_expressionBar->settingLinkage();
            if (m_expressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_memoryPublic->memoryplus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    case Qt::Key_Q:
        if (isPressCtrl && m_memCalbtn && !m_isallgray) {
            m_memoryKeypad->animate(MemoryKeypad::Key_Mminus);
            m_expressionBar->settingLinkage();
            if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
                m_memoryPublic->memoryminus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    case Qt::Key_M:
        if (isPressCtrl && m_memCalbtn && !m_isallgray) {
            m_memoryKeypad->animate(MemoryKeypad::Key_MS);
            m_expressionBar->settingLinkage();
            if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
                m_memoryPublic->generateData(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    default:
        break;
    }
    m_expressionBar->getInputEdit()->setFocus(); //edit 20200417 for bug--21146
}

/**
 * @brief 计算器键盘点击事件
 */
void BasicModule::handleKeypadButtonPress(int key)
{
    m_basicKeypad->update();
    m_expressionBar->clearSelection();
    //20200414 bug20294鼠标点击取消focus
    m_expressionBar->getInputEdit()->setFocus();
    QPair<QString, Quantity> p;
    switch (key) {
    case BasicKeypad::Key_0:
        m_expressionBar->enterNumberEvent("0"); //按键0事件
        break;
    case BasicKeypad::Key_1:
        m_expressionBar->enterNumberEvent("1");
        break;
    case BasicKeypad::Key_2:
        m_expressionBar->enterNumberEvent("2");
        break;
    case BasicKeypad::Key_3:
        m_expressionBar->enterNumberEvent("3");
        break;
    case BasicKeypad::Key_4:
        m_expressionBar->enterNumberEvent("4");
        break;
    case BasicKeypad::Key_5:
        m_expressionBar->enterNumberEvent("5");
        break;
    case BasicKeypad::Key_6:
        m_expressionBar->enterNumberEvent("6");
        break;
    case BasicKeypad::Key_7:
        m_expressionBar->enterNumberEvent("7");
        break;
    case BasicKeypad::Key_8:
        m_expressionBar->enterNumberEvent("8");
        break;
    case BasicKeypad::Key_9:
        m_expressionBar->enterNumberEvent("9");
        break;
    case BasicKeypad::Key_Plus:
        m_expressionBar->enterSymbolEvent("+");
        break;
    case BasicKeypad::Key_Min:
        m_expressionBar->enterSymbolEvent("-");
        break;
    case BasicKeypad::Key_Mult:
        m_expressionBar->enterSymbolEvent("*");
        break;
    case BasicKeypad::Key_Div:
        m_expressionBar->enterSymbolEvent("/");
        break;
    case BasicKeypad::Key_Percent:
        if (new_Func_Percent == 0)
            m_expressionBar->enterPercentEvent();
        else
            m_expressionBar->enterPercentEventCommon();
        break;
    case BasicKeypad::Key_Equals:
        m_expressionBar->settingLinkage();
        break;
    case BasicKeypad::Key_Clear:
        m_expressionBar->enterClearEvent();
        break;
    case BasicKeypad::Key_Backspace:
        m_expressionBar->enterBackspaceEvent();
        break;
    case BasicKeypad::Key_Point:
        m_expressionBar->enterPointEvent();
        break;
    case BasicKeypad::Key_Brackets:
        m_expressionBar->enterBracketsEvent();
        break;
    case MemoryKeypad::Key_MS:
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
            m_memoryPublic->generateData(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypad::Key_MC:
        m_memoryPublic->memoryclean();
        if (m_keypadLayout->currentIndex() == 1) {
            m_keypadLayout->setCurrentIndex(0);
            MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
            btn5->setbtnlight(false);
        }
        break;
    case MemoryKeypad::Key_Mlist:
        showListWidget();
        if (m_keypadLayout->currentIndex() == 1) {
            m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents); //鼠标穿透
            m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents); //鼠标穿透
        } else {
            m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        }
        m_memorylistwidget->setFocus();
        break;
    case MemoryKeypad::Key_Mplus:
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->memoryplus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypad::Key_Mminus:
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->memoryminus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypad::Key_MR:
        p = m_memorylistwidget->getfirstnumber();
        m_expressionBar->getInputEdit()->setAnswer(p.first, p.second);
        break;
    default:
        break;
    }
    m_expressionBar->addUndo();
}

void BasicModule::equalButtonPress()
{
    m_expressionBar->settingLinkage();
    m_expressionBar->addUndo();
}

/**
 * @brief 设置清楚按键状态
 */
void BasicModule::handleClearStateChanged(bool isAllClear)
{
    TextButton *btn = static_cast<TextButton *>(m_basicKeypad->button(BasicKeypad::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

/**
 * @brief mainwindow焦点不在basicmodul时也触发keypress
 */
void BasicModule::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

/**
 * @brief 向内存中存入数据时触发
 */
void BasicModule::mAvailableEvent()
{
    m_avail = true;
    MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
    btn->setEnabled(true);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
    btn1->setEnabled(true);
    MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
    btn2->setEnabled(true);
    m_memRCbtn = true;
}

/**
 * @brief 清空内存列表触发
 */
void BasicModule::mUnAvailableEvent()
{
    m_avail = false;
    MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
    btn->setEnabled(false);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
    btn1->setEnabled(false);
    m_memRCbtn = false;
    if (m_keypadLayout->currentIndex() == 0) {
        MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
        btn2->setEnabled(false);
    }
}

/**
 * @brief 显示内存列表
 */
void BasicModule::showListWidget()
{
    if (m_keypadLayout->currentIndex() == 0) {
        m_keypadLayout->setCurrentIndex(1);
        MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
        btn2->setbuttongray(true);
        MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
        btn3->setbuttongray(true);
        MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
        btn4->setbuttongray(true);
        MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
        btn->setbuttongray(true);
        MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
        btn1->setbuttongray(true);
        MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
        btn5->setbtnlight(true);
        m_isallgray = true;
    } /*else {
        m_keypadLayout->setCurrentIndex(0);
    }*/
}

void BasicModule::mousePressEvent(QMouseEvent *event)
{
    //内存界面显示时，点击内存界面以外部分切换内存界面为键盘界面
    if (m_keypadLayout->currentIndex() == 1 && m_insidewidget == false) {
        m_keypadLayout->setCurrentIndex(0);
        MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
        btn2->setbuttongray(false);
        MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
        btn3->setbuttongray(false);
        MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
        btn4->setbuttongray(false);
        MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
        btn->setbuttongray(false);
        MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
        btn1->setbuttongray(false);
        MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
        btn5->setbtnlight(false);
        m_isallgray = false;
        m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_expressionBar->getInputEdit()->setFocus();
    }

    if (m_avail == true) {
        MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
        btn->setEnabled(true);
        MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
        btn4->setEnabled(true);
        m_memRCbtn = true;
    } else {
        MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
        btn->setEnabled(false);
        MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
        btn1->setEnabled(false);
        MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
        btn5->setEnabled(false);
        m_memRCbtn = false;
    }
    m_insidewidget = false;
    m_expressionBar->getInputEdit()->isExpressionEmpty(); //确认输入栏是否有内容，发送信号M+,M-,MS是否置灰
    QWidget::mousePressEvent(event);
}
