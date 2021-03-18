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

#include "basicmoduletab.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "dthememanager.h"
#include "utils.h"
#include "math/quantity.h"

const int new_Func_Percent = 0; //0-旧的百分号方法 1-新

BasicModuleTab::BasicModuleTab(QWidget *parent)
    : DWidget(parent)
{
    m_scihiswidget = new SciHistoryWidgetTab(0, this);
    m_basicKeypad = new BasicKeypadTab(this);
    m_memoryKeypad = new MemoryKeypadTab(this);
    m_memCalbtn = false;
    m_memRCbtn = false;
    m_memoryPublic = MemoryPublicTab::instance(this);
    m_expressionBar = new ExpressionBarTab(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *leftlay = new QVBoxLayout();
    leftlay->addWidget(m_expressionBar);
    leftlay->addWidget(m_memoryKeypad);
    layout->addSpacing(1); //按钮边框多1pix
    leftlay->addWidget(m_basicKeypad);

    QVBoxLayout *rightlay = new QVBoxLayout();
    rightlay->addWidget(m_scihiswidget);
    layout->addLayout(leftlay);
    layout->addLayout(rightlay);
    layout->setSpacing(0);
    this->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);

    setMouseTracking(true);
    setFocus();

    m_expressionBar->initTheme(DGuiApplicationHelper::instance()->themeType());
    //信号槽连接
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_expressionBar, &ExpressionBarTab::initTheme);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this,
            &BasicModuleTab::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_basicKeypad, &BasicKeypadTab::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_memoryPublic, &MemoryPublicTab::setThemeType);
    connect(m_expressionBar, &ExpressionBarTab::keyPress, this, &BasicModuleTab::handleEditKeyPress);
    connect(m_expressionBar, &ExpressionBarTab::clearStateChanged, this,
            &BasicModuleTab::handleClearStateChanged);
    connect(m_basicKeypad, &BasicKeypadTab::buttonPressed, this,
            &BasicModuleTab::handleKeypadButtonPress);
    connect(m_basicKeypad, &BasicKeypadTab::buttonPressedbySpace, this,
            &BasicModuleTab::handleKeypadButtonPressByspace);
    connect(m_basicKeypad, &BasicKeypadTab::moveLeft, [ = ] { m_expressionBar->moveLeft(); });
    connect(m_basicKeypad, &BasicKeypadTab::moveRight, [ = ] { m_expressionBar->moveRight(); });
    connect(m_memoryKeypad, &MemoryKeypadTab::moveLeft, [ = ] { m_expressionBar->moveLeft(); });
    connect(m_memoryKeypad, &MemoryKeypadTab::moveRight, [ = ] { m_expressionBar->moveRight(); });
    connect(m_memoryKeypad, &MemoryKeypadTab::buttonPressed, this,
            &BasicModuleTab::handleKeypadButtonPress);
    connect(m_memoryKeypad, &MemoryKeypadTab::buttonPressedbySpace, this,
            &BasicModuleTab::handleKeypadButtonPressByspace);
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetplus, this, [ = ](int row) {
        //点击键盘按键上的m+,m-是先进行计算，若有计算结果放入内存中
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::widgetplus, m_expressionBar->getInputEdit()->getMemoryAnswer().second, row);
    });
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetminus, this, [ = ](int row) {
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::widgetminus, m_expressionBar->getInputEdit()->getMemoryAnswer().second, row);
    });
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::mListUnavailable, this, &BasicModuleTab::mUnAvailableEvent);
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::mListAvailable, this, &BasicModuleTab::mAvailableEvent);
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
        //内存界面点击item
        QString str = p.first;
        m_expressionBar->getInputEdit()->setAnswer(str.remove("\n"), p.second);
        m_expressionBar->getInputEdit()->setFocus();
        this->handleClearStateChanged(false);
        MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
        btn->setEnabled(true);
        MemoryButtonTab *btn1 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MR));
        btn1->setEnabled(true);
        MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mplus));
        btn2->setEnabled(true);
        MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mminus));
        btn3->setEnabled(true);
        MemoryButtonTab *btn4 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MS));
        btn4->setEnabled(true);
        m_memRCbtn = true;
        m_memCalbtn = true;
    });
    connect(m_expressionBar->getInputEdit(), &InputEdit::emptyExpression, this, [ = ](bool b) {
        if (b == false) {
            MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mplus));
            btn2->setEnabled(true);
            MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mminus));
            btn3->setEnabled(true);
            MemoryButtonTab *btn4 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MS));
            btn4->setEnabled(true);
            m_memCalbtn = true;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
        } else {
            MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mminus));
            btn3->setEnabled(false);
            MemoryButtonTab *btn4 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MS));
            btn4->setEnabled(false);
            m_memCalbtn = false;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
        }
    });
    //titlebar的清除按钮
    connect(m_scihiswidget, &SciHistoryWidgetTab::clearbtnShow, this, &BasicModuleTab::clearbtnShow);

    //获取科学模式内存是否为空，处理分开初始化科学模式下增加内存切到标准模式Mlist不能点击情况
    if (!m_memoryPublic->isWidgetEmpty(0))
        mAvailableEvent();
    else
        mUnAvailableEvent();
    setBasicTabOrder();
}

BasicModuleTab::~BasicModuleTab() {}

void BasicModuleTab::initTheme(int type)
{
    m_expressionBar->initTheme(type);
    update();
}

/**
 * @brief 物理键盘press事件
 */
void BasicModuleTab::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;
    const QString keyText = e->text();

    switch (e->key()) {
    case Qt::Key_0:
        m_expressionBar->enterNumberEvent("0"); //进入按键0事件
        m_basicKeypad->animate(BasicKeypadTab::Key_0); //按键0动画效果
        break;
    case Qt::Key_1:
        m_expressionBar->enterNumberEvent("1");
        m_basicKeypad->animate(BasicKeypadTab::Key_1);
        break;
    case Qt::Key_2:
        m_expressionBar->enterNumberEvent("2");
        m_basicKeypad->animate(BasicKeypadTab::Key_2);
        break;
    case Qt::Key_3:
        m_expressionBar->enterNumberEvent("3");
        m_basicKeypad->animate(BasicKeypadTab::Key_3);
        break;
    case Qt::Key_4:
        m_expressionBar->enterNumberEvent("4");
        m_basicKeypad->animate(BasicKeypadTab::Key_4);
        break;
    case Qt::Key_5:
        m_expressionBar->enterNumberEvent("5");
        m_basicKeypad->animate(BasicKeypadTab::Key_5);
        break;
    case Qt::Key_6:
        m_expressionBar->enterNumberEvent("6");
        m_basicKeypad->animate(BasicKeypadTab::Key_6);
        break;
    case Qt::Key_7:
        m_expressionBar->enterNumberEvent("7");
        m_basicKeypad->animate(BasicKeypadTab::Key_7);
        break;
    case Qt::Key_8:
        m_expressionBar->enterNumberEvent("8");
        m_basicKeypad->animate(BasicKeypadTab::Key_8);
        break;
    case Qt::Key_9:
        m_expressionBar->enterNumberEvent("9");
        m_basicKeypad->animate(BasicKeypadTab::Key_9);
        break;
    case Qt::Key_Plus:
        m_expressionBar->enterSymbolEvent("+");
        m_basicKeypad->animate(BasicKeypadTab::Key_Plus);
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_expressionBar->enterSymbolEvent("-");
        m_basicKeypad->animate(BasicKeypadTab::Key_Min);
        break;
    case Qt::Key_Asterisk:
        m_expressionBar->enterSymbolEvent("*");
        m_basicKeypad->animate(BasicKeypadTab::Key_Mult);
        break;
    case Qt::Key_Slash:
        m_expressionBar->enterSymbolEvent("/");
        m_basicKeypad->animate(BasicKeypadTab::Key_Div);
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        m_basicKeypad->animate(BasicKeypadTab::Key_Equals);
        m_expressionBar->settingLinkage();
//        setFocus();
        break;
    case Qt::Key_Backspace:
        m_expressionBar->enterBackspaceEvent();
        m_basicKeypad->animate(BasicKeypadTab::Key_Backspace);
        break;
    case Qt::Key_Period:
        m_basicKeypad->animate(BasicKeypadTab::Key_Point);
        m_expressionBar->enterPointEvent();
        break;
    case Qt::Key_Escape:
        m_expressionBar->enterClearEvent();
        m_basicKeypad->animate(BasicKeypadTab::Key_Clear);
        break;
    case Qt::Key_ParenLeft:
        m_basicKeypad->animate(BasicKeypadTab::Key_Brackets);
        m_expressionBar->enterLeftBracketsEvent();
        break;
    case Qt::Key_ParenRight:
        m_basicKeypad->animate(BasicKeypadTab::Key_Brackets);
        m_expressionBar->enterRightBracketsEvent();
        break;
    case Qt::Key_Percent:
        if (new_Func_Percent == 0)
            m_expressionBar->enterPercentEvent();
        else
            m_expressionBar->enterPercentEventCommon();
        m_basicKeypad->animate(BasicKeypadTab::Key_Percent);
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
            m_basicKeypad->animate(BasicKeypadTab::Key_Mult);
        }
        break;
    case Qt::Key_Delete:
        m_expressionBar->enterClearEvent();
        m_basicKeypad->animate(BasicKeypadTab::Key_Clear);
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
        if (isPressCtrl && m_memRCbtn) {
            m_memoryKeypad->animate(MemoryKeypadTab::Key_MC);
            QTimer::singleShot(100, this, [ = ] {
                m_memoryPublic->memoryclean(); //延迟，让动画效果显示
            });
        }
        break;
    case Qt::Key_R:
        if (isPressCtrl && m_memRCbtn) {
            m_memoryKeypad->animate(MemoryKeypadTab::Key_MR);
            m_expressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                       , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
            this->handleClearStateChanged(false);
        }
        break;
    case Qt::Key_P:
        if (isPressCtrl && m_memCalbtn) {
            m_memoryKeypad->animate(MemoryKeypadTab::Key_Mplus);
            m_expressionBar->settingLinkage();
            if (m_expressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_memoryPublic->memoryplus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    case Qt::Key_Q:
        if (isPressCtrl && m_memCalbtn) {
            m_memoryKeypad->animate(MemoryKeypadTab::Key_Mminus);
            m_expressionBar->settingLinkage();
            if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
                m_memoryPublic->memoryminus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    case Qt::Key_M:
        if (isPressCtrl && m_memCalbtn) {
            m_memoryKeypad->animate(MemoryKeypadTab::Key_MS);
            m_expressionBar->settingLinkage();
            if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
                m_memoryPublic->generateData(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    default:
        break;
    }
    if (!m_basicKeypad->buttonHasFocus() && !m_memoryKeypad->buttonHasFocus())
        m_expressionBar->getInputEdit()->setFocus(); //edit 20200417 for bug--21146
}

/**
 * @brief 计算器键盘点击事件
 */
void BasicModuleTab::handleKeypadButtonPress(int key)
{
    bool memoryfocus = false;
    m_basicKeypad->update();
    //20200414 bug20294鼠标点击取消focus
    switch (key) {
    case BasicKeypadTab::Key_0:
        m_expressionBar->enterNumberEvent("0"); //按键0事件
        break;
    case BasicKeypadTab::Key_1:
        m_expressionBar->enterNumberEvent("1");
        break;
    case BasicKeypadTab::Key_2:
        m_expressionBar->enterNumberEvent("2");
        break;
    case BasicKeypadTab::Key_3:
        m_expressionBar->enterNumberEvent("3");
        break;
    case BasicKeypadTab::Key_4:
        m_expressionBar->enterNumberEvent("4");
        break;
    case BasicKeypadTab::Key_5:
        m_expressionBar->enterNumberEvent("5");
        break;
    case BasicKeypadTab::Key_6:
        m_expressionBar->enterNumberEvent("6");
        break;
    case BasicKeypadTab::Key_7:
        m_expressionBar->enterNumberEvent("7");
        break;
    case BasicKeypadTab::Key_8:
        m_expressionBar->enterNumberEvent("8");
        break;
    case BasicKeypadTab::Key_9:
        m_expressionBar->enterNumberEvent("9");
        break;
    case BasicKeypadTab::Key_Plus:
        m_expressionBar->enterSymbolEvent("+");
        break;
    case BasicKeypadTab::Key_Min:
        m_expressionBar->enterSymbolEvent("-");
        break;
    case BasicKeypadTab::Key_Mult:
        m_expressionBar->enterSymbolEvent("*");
        break;
    case BasicKeypadTab::Key_Div:
        m_expressionBar->enterSymbolEvent("/");
        break;
    case BasicKeypadTab::Key_Percent:
        if (new_Func_Percent == 0)
            m_expressionBar->enterPercentEvent();
        else
            m_expressionBar->enterPercentEventCommon();
        break;
    case BasicKeypadTab::Key_Equals:
        m_expressionBar->settingLinkage();
        break;
    case BasicKeypadTab::Key_Clear:
        m_expressionBar->enterClearEvent();
        break;
    case BasicKeypadTab::Key_Backspace:
        m_expressionBar->enterBackspaceEvent();
        break;
    case BasicKeypadTab::Key_Point:
        m_expressionBar->enterPointEvent();
        break;
    case BasicKeypadTab::Key_Brackets:
        m_expressionBar->enterBracketsEvent();
        break;
    case MemoryKeypadTab::Key_MS:
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
            m_memoryPublic->generateData(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypadTab::Key_MC:
        m_memoryPublic->memoryclean();
        break;
    case MemoryKeypadTab::Key_Mplus:
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->memoryplus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypadTab::Key_Mminus:
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->memoryminus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypadTab::Key_MR:
        m_expressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                   , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
        this->handleClearStateChanged(false);
        break;
    default:
        break;
    }
    if (!memoryfocus)
        m_expressionBar->getInputEdit()->setFocus();
}

/**
 * @brief 焦点空格事件
 * @param key-button枚举值
 */
void BasicModuleTab::handleKeypadButtonPressByspace(int key)
{
    m_basicKeypad->update();
    m_memoryKeypad->update();
    //20200414 bug20294鼠标点击取消focus
//    m_expressionBar->getInputEdit()->setFocus();
    QPair<QString, Quantity> p;
    switch (key) {
    case BasicKeypadTab::Key_0:
        m_expressionBar->enterNumberEvent("0"); //按键0事件
        m_basicKeypad->animate(BasicKeypadTab::Key_0, true);
        break;
    case BasicKeypadTab::Key_1:
        m_expressionBar->enterNumberEvent("1");
        m_basicKeypad->animate(BasicKeypadTab::Key_1, true);
        break;
    case BasicKeypadTab::Key_2:
        m_expressionBar->enterNumberEvent("2");
        m_basicKeypad->animate(BasicKeypadTab::Key_2, true);
        break;
    case BasicKeypadTab::Key_3:
        m_expressionBar->enterNumberEvent("3");
        m_basicKeypad->animate(BasicKeypadTab::Key_3, true);
        break;
    case BasicKeypadTab::Key_4:
        m_expressionBar->enterNumberEvent("4");
        m_basicKeypad->animate(BasicKeypadTab::Key_4, true);
        break;
    case BasicKeypadTab::Key_5:
        m_expressionBar->enterNumberEvent("5");
        m_basicKeypad->animate(BasicKeypadTab::Key_5, true);
        break;
    case BasicKeypadTab::Key_6:
        m_expressionBar->enterNumberEvent("6");
        m_basicKeypad->animate(BasicKeypadTab::Key_6, true);
        break;
    case BasicKeypadTab::Key_7:
        m_expressionBar->enterNumberEvent("7");
        m_basicKeypad->animate(BasicKeypadTab::Key_7, true);
        break;
    case BasicKeypadTab::Key_8:
        m_expressionBar->enterNumberEvent("8");
        m_basicKeypad->animate(BasicKeypadTab::Key_8, true);
        break;
    case BasicKeypadTab::Key_9:
        m_expressionBar->enterNumberEvent("9");
        m_basicKeypad->animate(BasicKeypadTab::Key_9, true);
        break;
    case BasicKeypadTab::Key_Plus:
        m_expressionBar->enterSymbolEvent("+");
        m_basicKeypad->animate(BasicKeypadTab::Key_Plus, true);
        break;
    case BasicKeypadTab::Key_Min:
        m_expressionBar->enterSymbolEvent("-");
        m_basicKeypad->animate(BasicKeypadTab::Key_Min, true);
        break;
    case BasicKeypadTab::Key_Mult:
        m_expressionBar->enterSymbolEvent("*");
        m_basicKeypad->animate(BasicKeypadTab::Key_Mult, true);
        break;
    case BasicKeypadTab::Key_Div:
        m_expressionBar->enterSymbolEvent("/");
        m_basicKeypad->animate(BasicKeypadTab::Key_Div, true);
        break;
    case BasicKeypadTab::Key_Percent:
        if (new_Func_Percent == 0)
            m_expressionBar->enterPercentEvent();
        else
            m_expressionBar->enterPercentEventCommon();
        m_basicKeypad->animate(BasicKeypadTab::Key_Percent, true);
        break;
    case BasicKeypadTab::Key_Equals:
        m_expressionBar->settingLinkage();
        m_basicKeypad->animate(BasicKeypadTab::Key_Equals, true);
        break;
    case BasicKeypadTab::Key_Clear:
        m_expressionBar->enterClearEvent();
        m_basicKeypad->animate(BasicKeypadTab::Key_Clear, true);
        break;
    case BasicKeypadTab::Key_Backspace:
        m_expressionBar->enterBackspaceEvent();
        m_basicKeypad->animate(BasicKeypadTab::Key_Backspace, true);
        break;
    case BasicKeypadTab::Key_Point:
        m_expressionBar->enterPointEvent();
        m_basicKeypad->animate(BasicKeypadTab::Key_Point, true);
        break;
    case BasicKeypadTab::Key_Brackets:
        m_expressionBar->enterBracketsEvent();
        m_basicKeypad->animate(BasicKeypadTab::Key_Brackets, true);
        break;
    case MemoryKeypadTab::Key_MS:
        m_memoryKeypad->animate(MemoryKeypadTab::Key_MS, true);
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
            m_memoryPublic->generateData(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypadTab::Key_MC:
        m_memoryKeypad->animate(MemoryKeypadTab::Key_MC, true);
        m_memoryPublic->memoryclean();
        break;
    case MemoryKeypadTab::Key_Mplus:
        m_memoryKeypad->animate(MemoryKeypadTab::Key_Mplus, true);
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->memoryplus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypadTab::Key_Mminus:
        m_memoryKeypad->animate(MemoryKeypadTab::Key_Mminus, true);
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->memoryminus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemoryKeypadTab::Key_MR:
        m_memoryKeypad->animate(MemoryKeypadTab::Key_MR, true);
        m_expressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                   , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
        this->handleClearStateChanged(false);
        break;
    default:
        break;
    }
}

/**
 * @brief 设置清楚按键状态
 */
void BasicModuleTab::handleClearStateChanged(bool isAllClear)
{
    TextButtonTab *btn = static_cast<TextButtonTab *>(m_basicKeypad->button(BasicKeypadTab::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

void BasicModuleTab::setBasicTabOrder()
{
    this->setTabOrder(m_expressionBar->getInputEdit(), m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
    for (int i = 0; i < 4; i++) {
        this->setTabOrder(m_memoryKeypad->button(MemoryKeypadTab::Key_MC + i), m_memoryKeypad->button(MemoryKeypadTab::Key_MC + i + 1));
    }
    this->setTabOrder(m_memoryKeypad->button(MemoryKeypadTab::Key_Mminus), m_basicKeypad->button(BasicKeypadTab::Key_Clear));
    for (int i = 0; i < 20; i++) {
        this->setTabOrder(m_basicKeypad->button(BasicKeypadTab::Key_Clear + i), m_basicKeypad->button(BasicKeypadTab::Key_Clear + i + 1));
    }
    this->setTabOrder(m_basicKeypad->button(BasicKeypadTab::Key_Equals), static_cast<MemoryWidgetTab *>(m_scihiswidget->findChild<MemoryWidgetTab *>()));
    connect(static_cast<MemoryWidgetTab *>(m_scihiswidget->findChild<MemoryWidgetTab *>()), &MemoryWidgetTab::scimemtab, this, &BasicModuleTab::sciMemTab);
}

/**
 * @brief mainwindow焦点不在basicmodul时也触发keypress
 */
void BasicModuleTab::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

/**
 * @brief 向内存中存入数据时触发
 */
void BasicModuleTab::mAvailableEvent()
{
    m_avail = true;
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
    btn->setEnabled(true);
    MemoryButtonTab *btn1 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MR));
    btn1->setEnabled(true);
    m_memRCbtn = true;
}

/**
 * @brief 清空内存列表触发
 */
void BasicModuleTab::mUnAvailableEvent()
{
    m_avail = false;
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
    btn->setEnabled(false);
    btn->updateWhenBtnDisable();
    MemoryButtonTab *btn1 = static_cast<MemoryButtonTab *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MR));
    btn1->setEnabled(false);
    m_memRCbtn = false;
}

/**
 * @brief BasicModuleTab::titleBarClean
 * titlebar的删除按钮触发
 */
void BasicModuleTab::titleBarClean()
{
    m_memoryPublic->memoryclean();
    m_expressionBar->getInputEdit()->setFocus();
}

void BasicModuleTab::resetWindowSize(QSize size)
{
    m_basicKeypad->resetWidgetSize(size);
    m_memoryKeypad->resetWidgetSize(size);
    m_scihiswidget->resetWidgetSize(size);
}

void BasicModuleTab::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void BasicModuleTab::checkLineEmpty()
{
    m_expressionBar->getInputEdit()->isExpressionEmpty();
    m_scihiswidget->resetFocus();
}
