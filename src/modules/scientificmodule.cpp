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

#include "scientificmodule.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "dthememanager.h"
#include "src/utils.h"
#include "src/math/quantity.h"

scientificModule::scientificModule(QWidget *parent)
    : DWidget(parent)
{
    m_scikeypadwidget = new ScientificKeyPad;
    m_scihiswidget = new SciHistoryWidget(this);
    m_memCalbtn = false;
    m_memRCbtn = false;
    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *leftlay = new QVBoxLayout();
    m_sciexpressionBar = new SciExpressionBar;
    m_memoryPublic = MemoryPublic::instance(this);
    m_sciexpressionBar->setFixedHeight(95);
    leftlay->addWidget(m_sciexpressionBar);
    leftlay->addWidget(m_scikeypadwidget);
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
            m_sciexpressionBar, &SciExpressionBar::initTheme);

    connect(m_scihiswidget->m_listView, &SimpleListView::obtainingHistorical, this, [ = ](const QModelIndex & index) {
        m_sciexpressionBar->hisRevisionResults(index, m_scihiswidget->m_listModel->getAnswer(index.row()));
    }); //点击右侧历史记录
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged, this,
            &scientificModule::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scikeypadwidget, &ScientificKeyPad::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scihiswidget, &SciHistoryWidget::themeChanged);
    connect(m_sciexpressionBar, &SciExpressionBar::keyPress, this, &scientificModule::handleEditKeyPress);
    connect(m_sciexpressionBar, &SciExpressionBar::clearStateChanged, this,
            &scientificModule::handleClearStateChanged); //判断C/AC
    connect(m_sciexpressionBar, &SciExpressionBar::turnDeg, this, &scientificModule::handleDegChanged); //点击deg按键
    connect(m_sciexpressionBar, &SciExpressionBar::fEStateChanged, this, &scientificModule::handleFEStateChanged);
    connect(this, &scientificModule::changedeg, m_scikeypadwidget, &ScientificKeyPad::getdeg); //切换deg切图
    connect(m_scikeypadwidget, &ScientificKeyPad::buttonPressed, this,
            &scientificModule::handleKeypadButtonPress);
    connect(m_scikeypadwidget, &ScientificKeyPad::equalPressed, this, &scientificModule::equalButtonPress);
    connect(m_scikeypadwidget, &ScientificKeyPad::moveLeft, [ = ] { m_sciexpressionBar->moveLeft(); });
    connect(m_scikeypadwidget, &ScientificKeyPad::moveRight, [ = ] { m_sciexpressionBar->moveRight(); });
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::widgetplus, this, [ = ](int row) {
        //点击键盘按键上的m+,m-是先进行计算，若有计算结果放入内存中
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidget::widgetplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, row);
    });
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::widgetminus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidget::widgetminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, row);
    });
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::mListUnavailable, this, &scientificModule::mUnAvailableEvent);
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::mListAvailable, this, &scientificModule::mAvailableEvent);
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
        //内存界面点击item
        QString str = p.first;
        m_sciexpressionBar->getInputEdit()->setAnswer(str.remove("\n"), p.second);
        m_sciexpressionBar->getInputEdit()->setFocus();
        MemoryButton *btn = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MC));
        btn->setEnabled(true);
        MemoryButton *btn1 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MR));
        btn1->setEnabled(true);
        MemoryButton *btn2 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mplus));
        btn2->setEnabled(true);
        MemoryButton *btn3 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mmin));
        btn3->setEnabled(true);
        MemoryButton *btn4 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MS));
        btn4->setEnabled(true);
        m_memRCbtn = true;
        m_memCalbtn = true;
    });
    connect(m_sciexpressionBar->getInputEdit(), &InputEdit::emptyExpression, this, [ = ](bool b) {
        //输入栏为空m+,m-,ms置灰；否则可用
        if (b == false) {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mplus));
            btn2->setEnabled(true);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mmin));
            btn3->setEnabled(true);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MS));
            btn4->setEnabled(true);
            m_memCalbtn = true;
            m_scihiswidget->getMemoryWidget()->expressionempty(b);
        } else {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mmin));
            btn3->setEnabled(false);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MS));
            btn4->setEnabled(false);
            m_memCalbtn = false;
            m_scihiswidget->getMemoryWidget()->expressionempty(b);
        }
    });
    connect(this, &scientificModule::getWindowChanged, [ = ](int width, int height, bool hishide) {
        emit m_scikeypadwidget->windowSize(width, height - m_sciexpressionBar->height(), hishide);
    });
    connect(this, &scientificModule::getWindowChanged, [ = ] {
        m_sciexpressionBar->getInputEdit()->autoZoomFontSize(); //界面缩放时字体也进行缩放
    });

    if (!m_memoryPublic->isWidgetEmpty(1))
        mAvailableEvent();
    else
        mUnAvailableEvent();
}

scientificModule::~scientificModule() {}

void scientificModule::initTheme(int type)
{
    m_sciexpressionBar->initTheme(type);
    update();
}

/**
 * @brief 物理键盘事件
 */
void scientificModule::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier; //ctrl是否按下
    const bool isPressShift = e->modifiers() == Qt::ShiftModifier; //shift是否按下

    switch (e->key()) {
    case Qt::Key_0:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_0); //键盘按下动画效果
        if (!m_sciexpressionBar->judgeinput())
            return; //光标在函数中间直接return
        m_sciexpressionBar->enterNumberEvent("0");
        m_sciexpressionBar->addUndo(); //添加进撤销
        break;
    case Qt::Key_1:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_1);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("1");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_2:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_2);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("2");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_3:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_3);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("3");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_4:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_4);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("4");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_5:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_5);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("5");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_6:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_xy);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterxyEvent();
            m_sciexpressionBar->addUndo();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_6);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterNumberEvent("6");
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_7:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_7);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("7");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_8:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_8);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("8");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_9:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_9);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("9");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Plus:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Plus);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("+");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Min);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("-");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Asterisk:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Mult);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("*");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Slash:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Div);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("/");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        m_sciexpressionBar->enterEqualEvent();
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Equals);
        if (m_sciexpressionBar->getexpression().first) {
            m_scihiswidget->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidget->ishistoryfilled(true);
        }
        m_sciexpressionBar->addUndo();
        setFocus();
        break;
    case Qt::Key_Backspace:
        m_sciexpressionBar->enterBackspaceEvent();
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Backspace);
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Period:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Point);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPointEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Escape:
        m_sciexpressionBar->enterClearEvent();
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Clear);
        break;
    case Qt::Key_ParenLeft:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_left);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLeftBracketsEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_ParenRight:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_right);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterRightBracketsEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Percent:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Percent);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPercentEvent();
        m_sciexpressionBar->addUndo();
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
            m_sciexpressionBar->addUndo();
        } else {
            m_sciexpressionBar->enterFEEvent(m_FEisdown);
            m_scikeypadwidget->animate(ScientificKeyPad::Key_FE);
            m_sciexpressionBar->addUndo();
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
            m_scikeypadwidget->animate(ScientificKeyPad::Key_Mult);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterSymbolEvent("*");
            m_sciexpressionBar->addUndo();
        }
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Delete:
        m_sciexpressionBar->enterClearEvent();
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Clear);
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
            m_scikeypadwidget->animate(ScientificKeyPad::Key_xy);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterxyEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_L:
        if (isPressCtrl && m_memRCbtn) { //CTRL+L,MC
            m_scikeypadwidget->animate(ScientificKeyPad::Key_MC);
            QTimer::singleShot(100, this, [ = ] {
//                m_memorylistwidget->memoryclean();
                m_scihiswidget->memoryFunctions(SciHistoryWidget::memoryclean); //延迟，让动画效果显示
            });
        } else {
            if (isPressShift) {
                m_scikeypadwidget->animate(ScientificKeyPad::Key_logyx);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterlogyxEvent();
                m_sciexpressionBar->addUndo();
            } else {
                m_scikeypadwidget->animate(ScientificKeyPad::Key_log);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterLogEvent();
                m_sciexpressionBar->addUndo();
            }
        }
        break;
    case Qt::Key_R:
        if (isPressCtrl && m_memRCbtn) { //CTRL+R,MR
            m_scikeypadwidget->animate(ScientificKeyPad::Key_MR);
            m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->getMemoryWidget()->getfirstnumber().first
                                                          , m_scihiswidget->getMemoryWidget()->getfirstnumber().second);
        } else {
            if (isPressShift) {
                m_scikeypadwidget->animate(ScientificKeyPad::Key_Rand);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterRandEvent();
                m_sciexpressionBar->addUndo();
            } else {
                m_scikeypadwidget->animate(ScientificKeyPad::Key_Derivative);
                if (!m_sciexpressionBar->judgeinput())
                    return;
                m_sciexpressionBar->enterDerivativeEvent();
                m_sciexpressionBar->addUndo();
            }
        }
        break;
    case Qt::Key_P:
        if (isPressCtrl && m_memCalbtn) { //CTRL+P,M+
            m_sciexpressionBar->enterEqualEvent();
            m_scikeypadwidget->animate(ScientificKeyPad::Key_Mplus);
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_scihiswidget->memoryFunctions(SciHistoryWidget::memoryplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_PI);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterPIEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_Q:
        if (isPressCtrl && m_memCalbtn) { //CTRL+Q,M-
            m_sciexpressionBar->enterEqualEvent();
            m_scikeypadwidget->animate(ScientificKeyPad::Key_Mmin);
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_scihiswidget->memoryFunctions(SciHistoryWidget::memoryminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_x2);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterx2Event();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_M:
        if (isPressCtrl && m_memCalbtn) { //ctrl+m,MS
            m_sciexpressionBar->enterEqualEvent();
            m_scikeypadwidget->animate(ScientificKeyPad::Key_MS);
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first) //如果输入栏中可计算出结果
                m_scihiswidget->memoryFunctions(SciHistoryWidget::generateData, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_Mod);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterModEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_NumberSign: //#
        m_scikeypadwidget->animate(ScientificKeyPad::Key_x3);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx3Event();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_N:
        if (isPressCtrl) {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_ex);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterexEvent();
            m_sciexpressionBar->addUndo();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_ln);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterLnEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_At: //@
        m_scikeypadwidget->animate(ScientificKeyPad::Key_sqrt2);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSqrtEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_B:
        m_scikeypadwidget->animate(ScientificKeyPad::Key_sqrt3);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCbrtEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Bar: //|,shift+'\'
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Modulus);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModulusEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_E:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_e);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterEulerEvent();
            m_sciexpressionBar->addUndo();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_exp);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterExpEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_S:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_arcsin);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArcsinEvent();
            m_sciexpressionBar->addUndo();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_sin);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterSinEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_O:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_arccos);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArccosEvent();
            m_sciexpressionBar->addUndo();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_cos);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterCosEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_T:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_arctan);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArctanEvent();
            m_sciexpressionBar->addUndo();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_tan);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterTanEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_J:
        if (isPressShift) {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_arccot);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterArccotEvent();
            m_sciexpressionBar->addUndo();
        } else {
            m_scikeypadwidget->animate(ScientificKeyPad::Key_cot);
            if (!m_sciexpressionBar->judgeinput())
                return;
            m_sciexpressionBar->enterCotEvent();
            m_sciexpressionBar->addUndo();
        }
        break;
    case Qt::Key_Exclam: //'!'
        m_scikeypadwidget->animate(ScientificKeyPad::Key_Factorials);
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterFactorialsEvent();
        m_sciexpressionBar->addUndo();
        break;
    default:
        break;
    }
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
 * @brief 按钮时间
 */
void scientificModule::handleKeypadButtonPress(int key)
{
    m_sciexpressionBar->getInputEdit()->setFocus();
    m_scikeypadwidget->update();
    m_sciexpressionBar->clearSelection();
    //20200414 bug20294鼠标点击取消focus
    switch (key) {
    case ScientificKeyPad::Key_0:
        if (!m_sciexpressionBar->judgeinput())
            return; //光标在函数中直接return
        m_sciexpressionBar->enterNumberEvent("0");
        break;
    case ScientificKeyPad::Key_1:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("1");
        break;
    case ScientificKeyPad::Key_2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("2");
        break;
    case ScientificKeyPad::Key_3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("3");
        break;
    case ScientificKeyPad::Key_4:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("4");
        break;
    case ScientificKeyPad::Key_5:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("5");
        break;
    case ScientificKeyPad::Key_6:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("6");
        break;
    case ScientificKeyPad::Key_7:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("7");
        break;
    case ScientificKeyPad::Key_8:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("8");
        break;
    case ScientificKeyPad::Key_9:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterNumberEvent("9");
        break;
    case ScientificKeyPad::Key_Plus:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("+");
        break;
    case ScientificKeyPad::Key_Min:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("-");
        break;
    case ScientificKeyPad::Key_Mult:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("*");
        break;
    case ScientificKeyPad::Key_Div:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSymbolEvent("/");
        break;
    case ScientificKeyPad::Key_Percent:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPercentEvent();
        break;
    case ScientificKeyPad::Key_Equals:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getexpression().first) {
            m_scihiswidget->m_listModel->updataList(m_sciexpressionBar->getanswer(), m_sciexpressionBar->getexpression().second, 0);
            m_scihiswidget->ishistoryfilled(true);
        }
        break;
    case ScientificKeyPad::Key_Clear:
        m_sciexpressionBar->enterClearEvent();
        break;
    case ScientificKeyPad::Key_Backspace:
        m_sciexpressionBar->enterBackspaceEvent();
        break;
    case ScientificKeyPad::Key_Point:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPointEvent();
        break;
    case ScientificKeyPad::Key_left:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLeftBracketsEvent();
        break;
    case ScientificKeyPad::Key_right:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterRightBracketsEvent();
        break;
    case ScientificKeyPad::Key_MS:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidget::generateData, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case ScientificKeyPad::Key_MC:
        m_scihiswidget->memoryFunctions(SciHistoryWidget::memoryclean);
        break;
    case ScientificKeyPad::Key_Mplus:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidget::memoryplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case ScientificKeyPad::Key_Mmin:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidget::memoryminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case ScientificKeyPad::Key_MR:
        m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->getMemoryWidget()->getfirstnumber().first
                                                      , m_scihiswidget->getMemoryWidget()->getfirstnumber().second);
        break;
    case ScientificKeyPad::Key_deg:
        m_sciexpressionBar->enterDegEvent(m_deg);
        break;
    case ScientificKeyPad::Key_sin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSinEvent();
        break;
    case ScientificKeyPad::Key_FE:
        m_sciexpressionBar->enterFEEvent(m_FEisdown);
        break;
    case ScientificKeyPad::Key_page:
        handlePageStateChanged();
        break;
    case ScientificKeyPad::Key_PI:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterPIEvent();
        break;
    case ScientificKeyPad::Key_e:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterEulerEvent();
        break;
    case ScientificKeyPad::Key_Mod:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModEvent();
        break;
    case ScientificKeyPad::Key_x2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx2Event();
        break;
    case ScientificKeyPad::Key_Derivative:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterDerivativeEvent();
        break;
    case ScientificKeyPad::Key_Factorials:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterFactorialsEvent();
        break;
    case ScientificKeyPad::Key_exp:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterExpEvent();
        break;
    case ScientificKeyPad::Key_cos:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCosEvent();
        break;
    case ScientificKeyPad::Key_x3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterx3Event();
        break;
    case ScientificKeyPad::Key_tan:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterTanEvent();
        break;
    case ScientificKeyPad::Key_xy:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterxyEvent();
        break;
    case ScientificKeyPad::Key_cot:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCotEvent();
        break;
    case ScientificKeyPad::Key_10x:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enter10xEvent();
        break;
    case ScientificKeyPad::Key_Modulus:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterModulusEvent();
        break;
    case ScientificKeyPad::Key_log:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLogEvent();
        break;
    case ScientificKeyPad::Key_Rand:
        m_sciexpressionBar->enterRandEvent();
        break;
    case ScientificKeyPad::Key_ln:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterLnEvent();
        break;
    case ScientificKeyPad::Key_arcsin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArcsinEvent();
        break;
    case ScientificKeyPad::Key_sqrt2:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSqrtEvent();
        break;
    case ScientificKeyPad::Key_arccos:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArccosEvent();
        break;
    case ScientificKeyPad::Key_sqrt3:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterCbrtEvent();
        break;
    case ScientificKeyPad::Key_arctan:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArctanEvent();
        break;
    case ScientificKeyPad::Key_ysqrtx:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterYrootEvent();
        break;
    case ScientificKeyPad::Key_arccot:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterArccotEvent();
        break;
    case ScientificKeyPad::Key_2x:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enter2xEvent();
        break;
    case ScientificKeyPad::Key_logyx:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterlogyxEvent();
        break;
    case ScientificKeyPad::Key_ex:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterexEvent();
        break;
    case ScientificKeyPad::Key_opposite:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterOppositeEvent();
        break;
    default:
        break;
    }
    m_sciexpressionBar->addUndo();
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

void scientificModule::equalButtonPress()
{
    m_sciexpressionBar->enterEqualEvent();
    m_sciexpressionBar->addUndo();
}

/**
 * @brief 判断C/AC
 */
void scientificModule::handleClearStateChanged(bool isAllClear)
{
    TextButton *btn = static_cast<TextButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

/**
 * @brief 切换deg状态并发送deg切图信号
 */
void scientificModule::handleDegChanged()
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
void scientificModule::handleFEStateChanged(bool isdown)
{
    TextButton *btn = static_cast<TextButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_FE));
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
void scientificModule::handlePageStateChanged()
{
    TextButton *btn = static_cast<TextButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_page));
    if (m_Pageisdown) {
        m_Pageisdown = false;
    } else {
        m_Pageisdown = true;
    }
    btn->setButtonDown(m_Pageisdown);
}

/**
 * @brief 焦点不在scientificmodul时也触发keypress
 */
void scientificModule::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

/**
 * @brief memoryitem增加时mc,mr可用
 */
void scientificModule::mAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MC));
    btn->setEnabled(true);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MR));
    btn1->setEnabled(true);
    m_memRCbtn = true;
    m_avail = true;
}

/**
 * @brief mc后mc,mr不可用
 */
void scientificModule::mUnAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MC));
    btn->setEnabled(false);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MR));
    btn1->setEnabled(false);
    m_memRCbtn = false;
    m_avail = false;
}

/**
 * @brief 科学界面缩放情况和切换模式情况的历史记录是否显示
 */
void scientificModule::showOrHideHistory(bool hide)
{
    if (hide == true)
        m_scihiswidget->hide();
    else {
        m_scihiswidget->show();
        m_scihiswidget->focusOnButtonbox();
    }
}

/**
 * @brief 切到标准模式时在初始化进行判断m+,m-,ms是否可用
 */
void scientificModule::checkLineEmpty()
{
    m_sciexpressionBar->getInputEdit()->isExpressionEmpty();
}

