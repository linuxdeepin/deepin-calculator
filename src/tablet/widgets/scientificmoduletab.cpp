/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     xiajing <xiajing@uniontech.com>
*
* Maintainer: jingzhou <jingzhou@uniontech.com>
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

#include "scientificmoduletab.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "dthememanager.h"
#include "utils.h"
#include "math/quantity.h"

const int EXPRESSIONBAR_HEIGHT = 276;

scientificModuleTab::scientificModuleTab(QWidget *parent)
    : DWidget(parent)
{
    m_stackWidget = new QStackedWidget(this);
    m_memhiskeypad = new MemHisKeypadTab(this);
    m_scikeypadwidget = new ScientificKeyPadTab(this);
    m_scihiswidget = new SciHistoryWidgetTab(1, this);
    m_scihiswidgetver = new SciHistoryWidgetTab(3, this);
    m_memCalbtn = false;
    m_memRCbtn = false;
    m_sciexpressionBar = new SciExpressionBarTab(this);
    m_memoryPublic = MemoryPublicTab::instance();

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *leftlay = new QVBoxLayout();
    m_sciexpressionBar->setFixedHeight(EXPRESSIONBAR_HEIGHT);
    leftlay->addWidget(m_sciexpressionBar);
    leftlay->addWidget(m_memhiskeypad);
    leftlay->addWidget(m_stackWidget);
    m_stackWidget->addWidget(m_scihiswidgetver);
    m_stackWidget->addWidget(m_scikeypadwidget);
    leftlay->setSpacing(0);

    QVBoxLayout *rightlay = new QVBoxLayout();
    rightlay->addWidget(m_scihiswidget);
    layout->addLayout(leftlay);
    layout->addLayout(rightlay);
    layout->setSpacing(0);
    this->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);

    setMouseTracking(true); //默认false，默认下至少一个鼠标按键被按下才跟踪鼠标
    setFocus();

    m_sciexpressionBar->initTheme(DGuiApplicationHelper::instance()->themeType());
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_sciexpressionBar, &SciExpressionBarTab::initTheme);

    connect(m_scihiswidget->m_listView, &SimpleListViewTab::obtainingHistorical, this, [ = ](const QModelIndex & index) {
        m_sciexpressionBar->hisRevisionResults(index, m_scihiswidget->m_listModel->getAnswer(index.row()));
    }); //点击右侧历史记录
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this,
            &scientificModuleTab::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scikeypadwidget, &ScientificKeyPadTab::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scihiswidget, &SciHistoryWidgetTab::themeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_memoryPublic, &MemoryPublicTab::setThemeType);
    connect(m_sciexpressionBar, &SciExpressionBarTab::keyPress, this, &scientificModuleTab::handleEditKeyPress);
    connect(m_sciexpressionBar, &SciExpressionBarTab::clearStateChanged, this,
            &scientificModuleTab::handleClearStateChanged); //判断C/AC
    connect(m_sciexpressionBar, &SciExpressionBarTab::turnDeg, this, &scientificModuleTab::handleDegChanged); //点击deg按键
    connect(m_sciexpressionBar, &SciExpressionBarTab::fEStateChanged, this, &scientificModuleTab::handleFEStateChanged);
    connect(this, &scientificModuleTab::changedeg, m_scikeypadwidget, &ScientificKeyPadTab::getdeg); //切换deg切图
    connect(m_scikeypadwidget, &ScientificKeyPadTab::buttonPressed, this,
            &scientificModuleTab::handleKeypadButtonPress);
    connect(m_scikeypadwidget, &ScientificKeyPadTab::moveLeft, [ = ] { m_sciexpressionBar->moveLeft(); });
    connect(m_scikeypadwidget, &ScientificKeyPadTab::moveRight, [ = ] { m_sciexpressionBar->moveRight(); });
    connect(m_scikeypadwidget, &ScientificKeyPadTab::buttonPressedbySpace, this, &scientificModuleTab::handleKeypadButtonPressByspace);
    connect(m_scihiswidget, &SciHistoryWidgetTab::hisbtnClicked, [ = ] { m_sciexpressionBar->getInputEdit()->setFocus(); });
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetplus, this, [ = ](int row) {
        //点击键盘按键上的m+,m-是先进行计算，若有计算结果放入内存中
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::widgetplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, row);
    });
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetminus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::widgetminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, row);
    });
    connect(m_memoryPublic, &MemoryPublicTab::memorycleanSig, this, &scientificModuleTab::mUnAvailableEvent);
    connect(m_memoryPublic, &MemoryPublicTab::generateDataSig, this, &scientificModuleTab::mAvailableEvent);
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
        //内存界面点击item
        QString str = p.first;
        m_sciexpressionBar->getInputEdit()->setAnswer(str.remove("\n"), p.second);
        m_sciexpressionBar->getInputEdit()->setFocus();
        this->handleClearStateChanged(false);
        MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MC));
        btn->setEnabled(true);
        MemoryButtonTab *btn1 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MR));
        btn1->setEnabled(true);
        MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Mplus));
        btn2->setEnabled(true);
        MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Mmin));
        btn3->setEnabled(true);
        MemoryButtonTab *btn4 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MS));
        btn4->setEnabled(true);
        m_memRCbtn = true;
        m_memCalbtn = true;
    });
    connect(m_sciexpressionBar->getInputEdit(), &InputEdit::emptyExpression, this, [ = ](bool b) {
        //输入栏为空m+,m-,ms置灰；否则可用
        if (b == false) {
            MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Mplus));
            btn2->setEnabled(true);
            MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Mmin));
            btn3->setEnabled(true);
            MemoryButtonTab *btn4 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MS));
            btn4->setEnabled(true);
            MemoryButtonTab *btn5 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mplus));
            btn5->setEnabled(true);
            MemoryButtonTab *btn6 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mmin));
            btn6->setEnabled(true);
            MemoryButtonTab *btn7 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MS));
            btn7->setEnabled(true);
            m_memCalbtn = true;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
            m_scihiswidgetver->findChild<MemoryWidgetTab *>()->expressionempty(b);
        } else {
            MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Mmin));
            btn3->setEnabled(false);
            MemoryButtonTab *btn4 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MS));
            btn4->setEnabled(false);
            MemoryButtonTab *btn5 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mplus));
            btn5->setEnabled(false);
            MemoryButtonTab *btn6 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mmin));
            btn6->setEnabled(false);
            MemoryButtonTab *btn7 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MS));
            btn7->setEnabled(false);
            m_memCalbtn = false;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
            m_scihiswidgetver->findChild<MemoryWidgetTab *>()->expressionempty(b);
        }
    });

    //竖屏信号
    connect(m_memhiskeypad, &MemHisKeypadTab::buttonPressed, this,
            &scientificModuleTab::handleKeypadButtonPress);
    connect(m_memhiskeypad, &MemHisKeypadTab::buttonPressedbySpace, this, &scientificModuleTab::handleKeypadButtonPressByspace);
    connect(m_scihiswidgetver->m_listView, &SimpleListViewTab::obtainingHistorical, this, [ = ](const QModelIndex & index) {
        m_sciexpressionBar->hisRevisionResults(index, m_scihiswidgetver->m_listModel->getAnswer(index.row()));
    }); //点击右侧历史记录
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scihiswidgetver, &SciHistoryWidgetTab::themeChanged);
    connect(m_scihiswidget, &SciHistoryWidgetTab::hisIsFilled, [ = ](bool hisisfilled) {
        if (hisisfilled) {
            MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
            btn->setEnabled(true);
            m_havail = true;
        } else {
            m_havail = false;
        }
    });
    connect(m_scihiswidgetver, &SciHistoryWidgetTab::hisIsFilled, [ = ](bool hisisfilled) {
        if (hisisfilled) {
            MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
            btn->setEnabled(true);
            m_havail = true;
        } else {
            m_havail = false;
        }
    });
    connect(m_scihiswidgetver->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetplus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetplus(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
    });
    connect(m_scihiswidgetver->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetminus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetminus(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
    });
    connect(m_scihiswidgetver->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
        QString str = p.first;
        m_sciexpressionBar->getInputEdit()->setAnswer(str.remove("\n"), p.second);
        m_sciexpressionBar->getInputEdit()->setFocus();
        //点击item清除键状态改变
        this->handleClearStateChanged(false);
        m_stackWidget->setCurrentWidget(m_scikeypadwidget);
        MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
        btn->setbtnlight(false);
        m_memRCbtn = true;
        m_memCalbtn = true;
    });
    //同时删除历史记录
    connect(m_scihiswidget->m_listView, &SimpleListViewTab::removeRow, m_scihiswidgetver->m_listModel, &SimpleListModel::deleteItem);
    connect(m_scihiswidgetver->m_listView, &SimpleListViewTab::removeRow, m_scihiswidget->m_listModel, &SimpleListModel::deleteItem);

    //titlebar的清除按钮
    connect(m_scihiswidget, &SciHistoryWidgetTab::clearbtnShow, this, &scientificModuleTab::clearbtnShow);
    connect(m_scihiswidgetver, &SciHistoryWidgetTab::clearbtnShow, this, &scientificModuleTab::clearbtnShow);

    if (!m_memoryPublic->isEmpty())
        mAvailableEvent();
    else
        mUnAvailableEvent();
    setScientificTabOrder();
}

scientificModuleTab::~scientificModuleTab() {}

void scientificModuleTab::initTheme(int type)
{
    m_sciexpressionBar->initTheme(type);
    update();
}

/**
 * @brief 物理键盘事件
 */
void scientificModuleTab::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier; //ctrl是否按下
    const bool isPressShift = e->modifiers() == Qt::ShiftModifier; //shift是否按下

    switch (e->key()) {
    case Qt::Key_0:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_0); //键盘按下动画效果
        if (!m_sciexpressionBar->judgeinput())
            return; //光标在函数中间直接return
        m_sciexpressionBar->enterNumberEvent("0");
        break;
    case Qt::Key_1:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_1);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("1");
        break;
    case Qt::Key_2:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_2);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("2");
        break;
    case Qt::Key_3:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_3);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("3");
        break;
    case Qt::Key_4:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_4);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("4");
        break;
    case Qt::Key_5:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_5);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("5");
        break;
    case Qt::Key_6:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_6);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("6");
        break;
    case Qt::Key_7:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_7);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("7");
        break;
    case Qt::Key_8:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_8);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("8");
        break;
    case Qt::Key_9:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_9);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("9");
        break;
    case Qt::Key_Plus:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Plus);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("+");
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Min);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("-");
        break;
    case Qt::Key_Asterisk:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Mult);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("*");
        break;
    case Qt::Key_Slash:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Div);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("/");
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        m_sciexpressionBar->enterEqualEvent();
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Equals);
        if (m_sciexpressionBar->getexpression().first) {
            m_scihiswidget->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidget->historyfilled();
            m_scihiswidget->m_listView->scrollToTop();
            m_scihiswidgetver->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidgetver->historyfilled();
            m_scihiswidgetver->m_listView->scrollToTop();
        }
//        setFocus();
        break;
    case Qt::Key_Backspace:
        m_sciexpressionBar->enterBackspaceEvent();
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Backspace);
        break;
    case Qt::Key_Period:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Point);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPointEvent();
        break;
    case Qt::Key_Escape:
        m_sciexpressionBar->enterClearEvent();
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Clear);
        break;
    case Qt::Key_ParenLeft:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_left);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLeftBracketsEvent();
        break;
    case Qt::Key_ParenRight:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_right);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterRightBracketsEvent();
        break;
    case Qt::Key_Percent:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Percent);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPercentEvent();
        break;
    case Qt::Key_AsciiCircum:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_xy);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterxyEvent();
        break;
    case Qt::Key_C:
        if (isPressCtrl) {
            m_sciexpressionBar->copyResultToClipboard();
            setFocus();
        }
        break;
    case Qt::Key_V:
        if (isPressCtrl) {
            m_sciexpressionBar->copyClipboard2Result();
        } else {
            m_sciexpressionBar->enterFEEvent(m_FEisdown);
            if (m_scihiswidget->isHidden())
                m_memhiskeypad->animate(MemHisKeypadTab::Key_FE);
            else
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_FE);
        }
        break;
    case Qt::Key_A:
        if (isPressCtrl) {
            m_sciexpressionBar->allElection();
        }
        break;
    case Qt::Key_X:
        if (isPressCtrl) {
            m_sciexpressionBar->shear();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Mult);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterSymbolEvent("*");
        }
        break;
    case Qt::Key_Delete:
        m_sciexpressionBar->enterClearEvent();
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Clear);
        break;
    case Qt::Key_Left:
        m_sciexpressionBar->moveLeft();
        break;
    case Qt::Key_Right:
        m_sciexpressionBar->moveRight();
        break;
    case Qt::Key_Z:
        if (isPressCtrl)
            m_sciexpressionBar->Undo();
        break;
    case Qt::Key_Y:
        if (isPressCtrl)
            m_sciexpressionBar->Redo();
        else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_xy);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterxyEvent();
        }
        break;
    case Qt::Key_L:
        if (isPressCtrl && m_memRCbtn) { //CTRL+L,MC
            if (m_scihiswidget->isHidden())
                m_memhiskeypad->animate(MemHisKeypadTab::Key_MC);
            else
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_MC);
            QTimer::singleShot(100, this, [ = ] {
                m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryclean); //延迟，让动画效果显示
            });
        } else if (!isPressCtrl) {
            if (isPressShift) {
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_logyx);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterlogyxEvent();
            } else {
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_log);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterLogEvent();
            }
        }
        break;
    case Qt::Key_R:
        if (isPressCtrl && m_memRCbtn) { //CTRL+R,MR
            if (m_scihiswidget->isHidden())
                m_memhiskeypad->animate(MemHisKeypadTab::Key_MR);
            else
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_MR);
            m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                          , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
            this->handleClearStateChanged(false);
        } else if (!isPressCtrl) {
            if (isPressShift) {
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Rand);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterRandEvent();
            } else {
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Derivative);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterDerivativeEvent();
            }
        }
        break;
    case Qt::Key_P:
        if (isPressCtrl && m_memCalbtn) { //CTRL+P,M+
            m_sciexpressionBar->enterEqualEvent();
            if (m_scihiswidget->isHidden())
                m_memhiskeypad->animate(MemHisKeypadTab::Key_Mplus);
            else
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Mplus);
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        } else if (!isPressCtrl) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_PI);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterPIEvent();
        }
        break;
    case Qt::Key_Q:
        if (isPressCtrl && m_memCalbtn) { //CTRL+Q,M-
            m_sciexpressionBar->enterEqualEvent();
            if (m_scihiswidget->isHidden())
                m_memhiskeypad->animate(MemHisKeypadTab::Key_Mmin);
            else
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Mmin);
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        } else if (!isPressCtrl) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_x2);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterx2Event();
        }
        break;
    case Qt::Key_M:
        if (isPressCtrl && m_memCalbtn) { //ctrl+m,MS
            m_sciexpressionBar->enterEqualEvent();
            if (m_scihiswidget->isHidden())
                m_memhiskeypad->animate(MemHisKeypadTab::Key_MS);
            else
                m_scikeypadwidget->animate(ScientificKeyPadTab::Key_MS);
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::generateData, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        } else if (!isPressCtrl) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Mod);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterModEvent();
        }
        break;
    case Qt::Key_NumberSign: //#
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_x3);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx3Event();
        break;
    case Qt::Key_N:
        if (isPressCtrl) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_ex);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterexEvent();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_ln);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterLnEvent();
        }
        break;
    case Qt::Key_At: //@
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_sqrt2);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSqrtEvent();
        break;
    case Qt::Key_B:
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_sqrt3);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCbrtEvent();
        break;
    case Qt::Key_Bar: //|,shift+'\'
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Modulus);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModulusEvent();
        break;
    case Qt::Key_E:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_e);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterEulerEvent();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_exp);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterExpEvent();
        }
        break;
    case Qt::Key_S:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_arcsin);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArcsinEvent();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_sin);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterSinEvent();
        }
        break;
    case Qt::Key_O:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_arccos);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArccosEvent();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_cos);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterCosEvent();
        }
        break;
    case Qt::Key_T:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_arctan);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArctanEvent();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_tan);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterTanEvent();
        }
        break;
    case Qt::Key_J:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_arccot);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArccotEvent();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPadTab::Key_cot);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterCotEvent();
        }
        break;
    case Qt::Key_Exclam: //'!'
        m_scikeypadwidget->animate(ScientificKeyPadTab::Key_Factorials);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterFactorialsEvent();
        break;
    default:
        break;
    }
    if (!m_scikeypadwidget->buttonHasFocus())
        m_sciexpressionBar->getInputEdit()->setFocus(); //edit 20200417 for bug--21146
    int left = 0;
    int right = 0;
    QString text = m_sciexpressionBar->getInputEdit()->text();
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == "(")
            left ++;
        else if (text[i] == ")") {
            if (left > 0)
                left--;
            else
                right++;
        }
    }
    m_scikeypadwidget->bracketsNum(0, QString::number(left)); //写入左右括号不匹配数
    m_scikeypadwidget->bracketsNum(1, QString::number(right));
}

/**
 * @brief 按钮事件
 */
void scientificModuleTab::handleKeypadButtonPress(int key)
{
    m_scikeypadwidget->update();
    //20200414 bug20294鼠标点击取消focus
    int presskind = 0; //性能打点，0-数字点击 1-等于 2-内存操作 3-内存清空
    switch (key) {
    case ScientificKeyPadTab::Key_0:
        if (!m_sciexpressionBar->judgeinput())
            return; //光标在函数中直接return
        m_sciexpressionBar->enterNumberEvent("0");
        break;
    case ScientificKeyPadTab::Key_1:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("1");
        break;
    case ScientificKeyPadTab::Key_2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("2");
        break;
    case ScientificKeyPadTab::Key_3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("3");
        break;
    case ScientificKeyPadTab::Key_4:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("4");
        break;
    case ScientificKeyPadTab::Key_5:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("5");
        break;
    case ScientificKeyPadTab::Key_6:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("6");
        break;
    case ScientificKeyPadTab::Key_7:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("7");
        break;
    case ScientificKeyPadTab::Key_8:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("8");
        break;
    case ScientificKeyPadTab::Key_9:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("9");
        break;
    case ScientificKeyPadTab::Key_Plus:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("+");
        break;
    case ScientificKeyPadTab::Key_Min:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("-");
        break;
    case ScientificKeyPadTab::Key_Mult:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("*");
        break;
    case ScientificKeyPadTab::Key_Div:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("/");
        break;
    case ScientificKeyPadTab::Key_Percent:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPercentEvent();
        break;
    case ScientificKeyPadTab::Key_Equals:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getexpression().first) {
            m_scihiswidget->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidget->historyfilled();
            m_scihiswidget->m_listView->scrollToTop();
            m_scihiswidgetver->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidgetver->historyfilled();
            m_scihiswidgetver->m_listView->scrollToTop();
        }
        presskind = 1;
        break;
    case ScientificKeyPadTab::Key_Clear:
        m_sciexpressionBar->enterClearEvent();
        break;
    case ScientificKeyPadTab::Key_Backspace:
        m_sciexpressionBar->enterBackspaceEvent();
        break;
    case ScientificKeyPadTab::Key_Point:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPointEvent();
        break;
    case ScientificKeyPadTab::Key_left:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLeftBracketsEvent();
        break;
    case ScientificKeyPadTab::Key_right:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterRightBracketsEvent();
        break;
    case ScientificKeyPadTab::Key_MS:
    case MemHisKeypadTab::Key_MS:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::generateData, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        presskind = 2;
        break;
    case ScientificKeyPadTab::Key_MC:
    case MemHisKeypadTab::Key_MC:
        m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryclean);
        presskind = 3;
        break;
    case ScientificKeyPadTab::Key_Mplus:
    case MemHisKeypadTab::Key_Mplus:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        presskind = 2;
        break;
    case ScientificKeyPadTab::Key_Mmin:
    case MemHisKeypadTab::Key_Mmin:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        presskind = 2;
        break;
    case ScientificKeyPadTab::Key_MR:
    case MemHisKeypadTab::Key_MR:
        m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                      , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
        this->handleClearStateChanged(false);
        presskind = 2;
        break;
    case MemHisKeypadTab::Key_MHlist:
        if (m_stackWidget->currentWidget() == m_scikeypadwidget)
            showListWidget();
        else
            closeListWidget();
        break;
    case ScientificKeyPadTab::Key_deg:
        m_sciexpressionBar->enterDegEvent(m_deg);
        break;
    case ScientificKeyPadTab::Key_sin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSinEvent();
        break;
    case ScientificKeyPadTab::Key_FE:
    case MemHisKeypadTab::Key_FE:
        m_sciexpressionBar->enterFEEvent(m_FEisdown);
        break;
    case ScientificKeyPadTab::Key_page:
        handlePageStateChanged();
        break;
    case ScientificKeyPadTab::Key_PI:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPIEvent();
        break;
    case ScientificKeyPadTab::Key_e:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterEulerEvent();
        break;
    case ScientificKeyPadTab::Key_Mod:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModEvent();
        break;
    case ScientificKeyPadTab::Key_x2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx2Event();
        break;
    case ScientificKeyPadTab::Key_Derivative:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterDerivativeEvent();
        break;
    case ScientificKeyPadTab::Key_Factorials:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterFactorialsEvent();
        break;
    case ScientificKeyPadTab::Key_exp:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterExpEvent();
        break;
    case ScientificKeyPadTab::Key_cos:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCosEvent();
        break;
    case ScientificKeyPadTab::Key_x3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx3Event();
        break;
    case ScientificKeyPadTab::Key_tan:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterTanEvent();
        break;
    case ScientificKeyPadTab::Key_xy:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterxyEvent();
        break;
    case ScientificKeyPadTab::Key_cot:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCotEvent();
        break;
    case ScientificKeyPadTab::Key_10x:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enter10xEvent();
        break;
    case ScientificKeyPadTab::Key_Modulus:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModulusEvent();
        break;
    case ScientificKeyPadTab::Key_log:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLogEvent();
        break;
    case ScientificKeyPadTab::Key_Rand:
        m_sciexpressionBar->enterRandEvent();
        break;
    case ScientificKeyPadTab::Key_ln:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLnEvent();
        break;
    case ScientificKeyPadTab::Key_arcsin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArcsinEvent();
        break;
    case ScientificKeyPadTab::Key_sqrt2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSqrtEvent();
        break;
    case ScientificKeyPadTab::Key_arccos:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArccosEvent();
        break;
    case ScientificKeyPadTab::Key_sqrt3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCbrtEvent();
        break;
    case ScientificKeyPadTab::Key_arctan:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArctanEvent();
        break;
    case ScientificKeyPadTab::Key_ysqrtx:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterYrootEvent();
        break;
    case ScientificKeyPadTab::Key_arccot:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArccotEvent();
        break;
    case ScientificKeyPadTab::Key_2x:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enter2xEvent();
        break;
    case ScientificKeyPadTab::Key_logyx:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterlogyxEvent();
        break;
    case ScientificKeyPadTab::Key_ex:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterexEvent();
        break;
    case ScientificKeyPadTab::Key_opposite:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterOppositeEvent();
        break;
    default:
        break;
    }
    m_sciexpressionBar->getInputEdit()->setFocus();
    int left = 0;
    int right = 0;
    QString text = m_sciexpressionBar->getInputEdit()->text();
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == "(")
            left ++;
        else if (text[i] == ")") {
            if (left > 0)
                left--;
            else
                right++;
        }
    }
    m_scikeypadwidget->bracketsNum(0, QString::number(left)); //写入左右括号不匹配数
    m_scikeypadwidget->bracketsNum(1, QString::number(right));
//    switch (presskind) {
//    case 0:
//        PerformanceMonitor::finishOperate("数字键盘点击");
//        break;
//    case 1:
//        PerformanceMonitor::finishOperate("运算");
//        break;
//    case 2:
//        PerformanceMonitor::finishOperate("内存操作");
//        break;
//    case 3:
//        PerformanceMonitor::finishOperate("内存清空");
//        break;
//    default:
//        PerformanceMonitor::finishOperate("数字键盘点击");
//        break;
//    }
}

void scientificModuleTab::handleKeypadButtonPressByspace(int key)
{
    m_scikeypadwidget->update();
    m_memhiskeypad->update();
    if (key > 83 && key < 91)
        m_memhiskeypad->animate(MemHisKeypadTab::Buttons(key), true);
    else if (key < 84)
        m_scikeypadwidget->animate(ScientificKeyPadTab::Buttons(key), true);
    //20200414 bug20294鼠标点击取消focus
    switch (key) {
    case ScientificKeyPadTab::Key_0:
        if (!m_sciexpressionBar->judgeinput())
            return; //光标在函数中直接return
        m_sciexpressionBar->enterNumberEvent("0");
        break;
    case ScientificKeyPadTab::Key_1:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("1");
        break;
    case ScientificKeyPadTab::Key_2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("2");
        break;
    case ScientificKeyPadTab::Key_3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("3");
        break;
    case ScientificKeyPadTab::Key_4:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("4");
        break;
    case ScientificKeyPadTab::Key_5:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("5");
        break;
    case ScientificKeyPadTab::Key_6:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("6");
        break;
    case ScientificKeyPadTab::Key_7:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("7");
        break;
    case ScientificKeyPadTab::Key_8:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("8");
        break;
    case ScientificKeyPadTab::Key_9:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("9");
        break;
    case ScientificKeyPadTab::Key_Plus:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("+");
        break;
    case ScientificKeyPadTab::Key_Min:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("-");
        break;
    case ScientificKeyPadTab::Key_Mult:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("*");
        break;
    case ScientificKeyPadTab::Key_Div:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("/");
        break;
    case ScientificKeyPadTab::Key_Percent:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPercentEvent();
        break;
    case ScientificKeyPadTab::Key_Equals:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getexpression().first) {
            m_scihiswidget->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidget->historyfilled();
            m_scihiswidget->m_listView->scrollToTop();
            m_scihiswidgetver->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidgetver->historyfilled();
            m_scihiswidgetver->m_listView->scrollToTop();
        }
        break;
    case ScientificKeyPadTab::Key_Clear:
        m_sciexpressionBar->enterClearEvent();
        break;
    case ScientificKeyPadTab::Key_Backspace:
        m_sciexpressionBar->enterBackspaceEvent();
        break;
    case ScientificKeyPadTab::Key_Point:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPointEvent();
        break;
    case ScientificKeyPadTab::Key_left:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLeftBracketsEvent();
        break;
    case ScientificKeyPadTab::Key_right:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterRightBracketsEvent();
        break;
    case ScientificKeyPadTab::Key_MS:
    case MemHisKeypadTab::Key_MS:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::generateData, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case ScientificKeyPadTab::Key_MC:
    case MemHisKeypadTab::Key_MC:
        m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryclean);
        break;
    case ScientificKeyPadTab::Key_Mplus:
    case MemHisKeypadTab::Key_Mplus:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case ScientificKeyPadTab::Key_Mmin:
    case MemHisKeypadTab::Key_Mmin:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case ScientificKeyPadTab::Key_MR:
    case MemHisKeypadTab::Key_MR:
        m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                      , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
        this->handleClearStateChanged(false);
        break;
    case MemHisKeypadTab::Key_MHlist:
        if (m_stackWidget->currentWidget() == m_scikeypadwidget)
            showListWidget();
        else
            closeListWidget();
        break;
    case ScientificKeyPadTab::Key_deg:
        m_sciexpressionBar->enterDegEvent(m_deg);
        break;
    case ScientificKeyPadTab::Key_sin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSinEvent();
        break;
    case ScientificKeyPadTab::Key_FE:
    case MemHisKeypadTab::Key_FE:
        m_sciexpressionBar->enterFEEvent(m_FEisdown);
        break;
    case ScientificKeyPadTab::Key_page:
        handlePageStateChanged();
        break;
    case ScientificKeyPadTab::Key_PI:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPIEvent();
        break;
    case ScientificKeyPadTab::Key_e:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterEulerEvent();
        break;
    case ScientificKeyPadTab::Key_Mod:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModEvent();
        break;
    case ScientificKeyPadTab::Key_x2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx2Event();
        break;
    case ScientificKeyPadTab::Key_Derivative:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterDerivativeEvent();
        break;
    case ScientificKeyPadTab::Key_Factorials:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterFactorialsEvent();
        break;
    case ScientificKeyPadTab::Key_exp:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterExpEvent();
        break;
    case ScientificKeyPadTab::Key_cos:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCosEvent();
        break;
    case ScientificKeyPadTab::Key_x3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx3Event();
        break;
    case ScientificKeyPadTab::Key_tan:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterTanEvent();
        break;
    case ScientificKeyPadTab::Key_xy:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterxyEvent();
        break;
    case ScientificKeyPadTab::Key_cot:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCotEvent();
        break;
    case ScientificKeyPadTab::Key_10x:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enter10xEvent();
        break;
    case ScientificKeyPadTab::Key_Modulus:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModulusEvent();
        break;
    case ScientificKeyPadTab::Key_log:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLogEvent();
        break;
    case ScientificKeyPadTab::Key_Rand:
        m_sciexpressionBar->enterRandEvent();
        break;
    case ScientificKeyPadTab::Key_ln:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLnEvent();
        break;
    case ScientificKeyPadTab::Key_arcsin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArcsinEvent();
        break;
    case ScientificKeyPadTab::Key_sqrt2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSqrtEvent();
        break;
    case ScientificKeyPadTab::Key_arccos:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArccosEvent();
        break;
    case ScientificKeyPadTab::Key_sqrt3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCbrtEvent();
        break;
    case ScientificKeyPadTab::Key_arctan:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArctanEvent();
        break;
    case ScientificKeyPadTab::Key_ysqrtx:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterYrootEvent();
        break;
    case ScientificKeyPadTab::Key_arccot:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArccotEvent();
        break;
    case ScientificKeyPadTab::Key_2x:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enter2xEvent();
        break;
    case ScientificKeyPadTab::Key_logyx:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterlogyxEvent();
        break;
    case ScientificKeyPadTab::Key_ex:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterexEvent();
        break;
    case ScientificKeyPadTab::Key_opposite:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterOppositeEvent();
        break;
    default:
        break;
    }
    int left = 0;
    int right = 0;
    QString text = m_sciexpressionBar->getInputEdit()->text();
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == "(")
            left ++;
        else if (text[i] == ")") {
            if (left > 0)
                left--;
            else
                right++;
        }
    }
    m_scikeypadwidget->bracketsNum(0, QString::number(left)); //写入左右括号不匹配数
    m_scikeypadwidget->bracketsNum(1, QString::number(right));
}

/**
 * @brief 判断C/AC
 */
void scientificModuleTab::handleClearStateChanged(bool isAllClear)
{
    TextButtonTab *btn = static_cast<TextButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

/**
 * @brief 切换deg状态并发送deg切图信号
 */
void scientificModuleTab::handleDegChanged()
{
    emit changedeg(m_deg);
    if (m_deg == 1) {
        m_deg = 2;
    } else if (m_deg == 2) {
        m_deg = 3;
    } else if (m_deg == 3) {
        m_deg = 1;
    }
}

/**
 * @brief FE是否被按下
 */
void scientificModuleTab::handleFEStateChanged(bool isdown)
{
    TextButtonTab *btn = m_scihiswidget->isHidden() ? static_cast<TextButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_FE))
                         : static_cast<TextButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE));
    if (isdown) {
        m_FEisdown = true;
    } else {
        m_FEisdown = false;
    }
    btn->setButtonDown(m_FEisdown);
}

/**
 * @brief 切换2nd状态
 */
void scientificModuleTab::handlePageStateChanged()
{
    TextButtonTab *btn = static_cast<TextButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_page));
    if (m_Pageisdown) {
        m_Pageisdown = false;
    } else {
        m_Pageisdown = true;
    }
    btn->setButtonDown(m_Pageisdown);
}

/**
 * @brief scientificModuleTab::setScientificTabOrder
 * tab事件排序
 */
void scientificModuleTab::setScientificTabOrder()
{
    this->setTabOrder(m_sciexpressionBar->getInputEdit(), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE));
    for (int i = 0; i < 47; i++) {
        switch (ScientificKeyPadTab::Key_FE + i) {
        case 38:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_arcsin));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_arcsin), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 39:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_sqrt2));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_sqrt2), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 44:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_arccos));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_arccos), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 45:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_sqrt3));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_sqrt3), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 50:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_arctan));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_arctan), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 51:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_ysqrtx));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_ysqrtx), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 56:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_arccot));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_arccot), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 57:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_2x));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_2x), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 63:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_logyx));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_logyx), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        case 69:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_ex));
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_ex), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
            break;
        default:
            this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i), m_scikeypadwidget->button(ScientificKeyPadTab::Key_FE + i + 1));
        }
    }
    this->setTabOrder(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Equals)
                      , static_cast<DButtonBoxButton *>(m_scihiswidget->findChild<DButtonBox *>()->button(0)));

    this->setTabOrder(static_cast<DButtonBoxButton *>(m_scihiswidget->findChild<DButtonBox *>()->button(0)),
                      static_cast<DButtonBoxButton *>(m_scihiswidget->findChild<DButtonBox *>()->button(1)));

    this->setTabOrder(static_cast<DButtonBoxButton *>(m_scihiswidget->findChild<DButtonBox *>()->button(1))
                      , static_cast<DPushButton *>(m_scihiswidget->findChild<IconButtonTab *>("clearbtn")));

    this->setTabOrder(static_cast<DPushButton *>(m_scihiswidget->findChild<IconButtonTab *>("clearbtn"))
                      , static_cast<DListView *>(m_scihiswidget->findChild<SimpleListViewTab *>()));

    this->setTabOrder(static_cast<DListView *>(m_scihiswidget->findChild<SimpleListViewTab *>())
                      , static_cast<MemoryWidgetTab *>(m_scihiswidget->findChild<MemoryWidgetTab *>()));
    connect(static_cast<MemoryWidgetTab *>(m_scihiswidget->findChild<MemoryWidgetTab *>()), &MemoryWidgetTab::scimemtab, this, &scientificModuleTab::sciMemTab);
}

/**
 * @brief 焦点不在scientificmodul时也触发keypress
 */
void scientificModuleTab::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

/**
 * @brief memoryitem增加时mc,mr可用
 */
void scientificModuleTab::mAvailableEvent()
{
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MC));
    btn->setEnabled(true);
    MemoryButtonTab *btn1 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MR));
    btn1->setEnabled(true);
    MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
    btn2->setEnabled(true);
    MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
    btn3->setEnabled(true);
    MemoryButtonTab *btn4 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
    btn4->setEnabled(true);
    m_memRCbtn = true;
    m_avail = true;
}

/**
 * @brief mc后mc,mr不可用
 */
void scientificModuleTab::mUnAvailableEvent()
{
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MC));
    btn->setEnabled(false);
    btn->updateWhenBtnDisable();
    MemoryButtonTab *btn1 = static_cast<MemoryButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_MR));
    btn1->setEnabled(false);
    MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
    btn2->setEnabled(false);
    MemoryButtonTab *btn3 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
    btn3->setEnabled(false);
    m_memRCbtn = false;
    m_avail = false;
    if (m_stackWidget->currentWidget() == m_scikeypadwidget && m_havail == false) {
        MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
        btn2->setEnabled(false);
    }
}

void scientificModuleTab::showListWidget()
{
    m_stackWidget->setCurrentWidget(m_scihiswidgetver);
    if (static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist))->hasFocus()) {
        m_scihiswidgetver->findChild<DButtonBoxButton *>("mButtonBoxButton")->setFocus();
    }
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
    btn->setbtnlight(true);
}

void scientificModuleTab::closeListWidget()
{
    m_stackWidget->setCurrentWidget(m_scikeypadwidget);
    m_sciexpressionBar->getInputEdit()->setFocus();
    if (m_avail == true) {
        MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
        btn->setbuttongray(false);
        btn->setEnabled(true);
        MemoryButtonTab *btn7 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
        btn7->setbuttongray(false);
        btn7->setEnabled(true);
        m_memRCbtn = true;
    } else {
        MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
        btn->setbuttongray(true);
        btn->setEnabled(false);
        MemoryButtonTab *btn1 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
        btn->setbuttongray(true);
        btn1->setEnabled(false);
        if (m_havail == false) {
            MemoryButtonTab *btn8 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
            btn8->setEnabled(false);
        }
        m_memRCbtn = false;
    }
    MemoryButtonTab *btn2 = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
    btn2->setbtnlight(false);
    m_sciexpressionBar->getInputEdit()->isExpressionEmpty(); //确认输入栏是否有内容，发送信号M+,M-,MS是否置灰
}

/**
 * @brief scientificModuleTab::titleBarClean
 * titlebar的删除按钮触发
 */
void scientificModuleTab::titleBarClean()
{
    m_scihiswidget->cleanButtonEvent();
    m_scihiswidgetver->cleanButtonEvent();
    m_sciexpressionBar->getInputEdit()->setFocus();
}

void scientificModuleTab::resetWindowSize(QSize size)
{
    m_scikeypadwidget->resetWidgetSize(size);
    m_scihiswidget->resetWidgetSize(size);
    if (size.width() < size.height()) {
        m_scihiswidget->hide();
        m_memhiskeypad->show();
        m_memhiskeypad->resetWidgetSize(size);
        m_scihiswidgetver->resetWidgetSize(size);
        m_sciexpressionBar->setFixedHeight(500 * size.height() / 1880);
    } else {
        m_memhiskeypad->hide();
        m_scihiswidget->resetWidgetSize(size);
        m_scihiswidget->show();
        m_sciexpressionBar->setFixedHeight(EXPRESSIONBAR_HEIGHT * size.height() / 1055);
    }
    m_stackWidget->setCurrentWidget(m_scikeypadwidget);
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
    btn->setbtnlight(false);
    m_sciexpressionBar->resetWidgetSize(size);
}

/**
 * @brief 切到标准模式时在初始化进行判断m+,m-,ms是否可用
 */
void scientificModuleTab::checkLineEmpty()
{
    m_sciexpressionBar->getInputEdit()->isExpressionEmpty();
    m_scihiswidget->resetFocus();
//    m_scihiswidgetver->resetFocus();
}

