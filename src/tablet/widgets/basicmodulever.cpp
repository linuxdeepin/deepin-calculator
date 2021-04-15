#include "basicmodulever.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "dthememanager.h"
#include "utils.h"
#include "math/quantity.h"

BasicModuleVer::BasicModuleVer(QWidget *parent)
    : DWidget(parent)
{
    m_basicKeypad = new BasicKeypadTab(this);
    m_memoryKeypad = new MemoryKeypadTab(this);
    m_keypadLayout = new QStackedWidget(this);
    m_memCalbtn = false;
    m_memRCbtn = false;
    m_memoryPublic = MemoryPublicTab::instance();
    m_scihiswidget = new SciHistoryWidgetTab(2, this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_expressionBar = new ExpressionBarTab(this);
    layout->addWidget(m_expressionBar);
    layout->addWidget(m_memoryKeypad);
    layout->addSpacing(1); //按钮边框多1pix
    layout->addWidget(m_keypadLayout);
    m_keypadLayout->addWidget(m_basicKeypad);
    m_keypadLayout->addWidget(m_scihiswidget);

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setMouseTracking(true);
    setFocus();
    m_expressionBar->initTheme(DGuiApplicationHelper::instance()->themeType());
    //信号槽连接
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_expressionBar, &ExpressionBarTab::initTheme);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this,
            &BasicModuleVer::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_basicKeypad, &BasicKeypadTab::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_memoryPublic, &MemoryPublicTab::setThemeType);
    connect(m_expressionBar, &ExpressionBarTab::keyPress, this, &BasicModuleVer::handleEditKeyPress);
    connect(m_expressionBar, &ExpressionBarTab::clearStateChanged, this,
            &BasicModuleVer::handleClearStateChanged);
    connect(m_basicKeypad, &BasicKeypadTab::buttonPressed, this,
            &BasicModuleVer::handleKeypadButtonPress);
    connect(m_basicKeypad, &BasicKeypadTab::buttonPressedbySpace, this,
            &BasicModuleVer::handleKeypadButtonPressByspace);
    connect(m_basicKeypad, &BasicKeypadTab::moveLeft, [ = ] { m_expressionBar->moveLeft(); });
    connect(m_basicKeypad, &BasicKeypadTab::moveRight, [ = ] { m_expressionBar->moveRight(); });
    connect(m_memoryKeypad, &MemoryKeypadTab::moveLeft, [ = ] { m_expressionBar->moveLeft(); });
    connect(m_memoryKeypad, &MemoryKeypadTab::moveRight, [ = ] { m_expressionBar->moveRight(); });
    connect(m_memoryKeypad, &MemoryKeypadTab::buttonPressed, this,
            &BasicModuleVer::handleKeypadButtonPress);
    connect(m_memoryKeypad, &MemoryKeypadTab::buttonPressedbySpace, this,
            &BasicModuleVer::handleKeypadButtonPressByspace);
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetplus, this, [ = ](int row) {
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetplus(row, m_expressionBar->getInputEdit()->getMemoryAnswer().second);
    });
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetminus, this, [ = ](int row) {
        m_expressionBar->settingLinkage();
        if (m_expressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetminus(row, m_expressionBar->getInputEdit()->getMemoryAnswer().second);
    });

    connect(m_memoryPublic, &MemoryPublicTab::memorycleanSig, this, &BasicModuleVer::mUnAvailableEvent);
    connect(m_memoryPublic, &MemoryPublicTab::generateDataSig, this, &BasicModuleVer::mAvailableEvent);
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
        QString str = p.first;
        m_expressionBar->getInputEdit()->setAnswer(str.remove("\n"), p.second);
        m_expressionBar->getInputEdit()->setFocus();
        //点击item清除键状态改变
        this->handleClearStateChanged(false);
        m_keypadLayout->setCurrentIndex(0);
    });
    connect(m_expressionBar->getInputEdit(), &InputEdit::emptyExpression, this, [ = ](bool b) {
        if (b == false) {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mplus));
            btn2->setEnabled(true);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mminus));
            btn3->setEnabled(true);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MS));
            btn4->setEnabled(true);
            m_memCalbtn = true;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
        } else {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mminus));
            btn3->setEnabled(false);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MS));
            btn4->setEnabled(false);
            m_memCalbtn = false;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
        }
    });
    //titlebar的清除按钮
    connect(m_scihiswidget, &SciHistoryWidgetTab::clearbtnShow, this, &BasicModuleVer::clearbtnShow);
    //获取科学模式内存是否为空，处理分开初始化科学模式下增加内存切到标准模式Mlist不能点击情况
    if (!m_memoryPublic->isEmpty())
        mAvailableEvent();
    else
        mUnAvailableEvent();
    setBasicTabOrder();
}

BasicModuleVer::~BasicModuleVer() {}

void BasicModuleVer::initTheme(int type)
{
    m_expressionBar->initTheme(type);
    update();
}

/**
 * @brief 物理键盘press事件
 */
void BasicModuleVer::handleEditKeyPress(QKeyEvent *e)
{
    if (m_keypadLayout->currentIndex() == 1)
        return;
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
        m_expressionBar->enterPercentEvent();
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
            m_expressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first,
                                                       m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
            //点击item清除键状态改变
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
void BasicModuleVer::handleKeypadButtonPress(int key)
{
    bool pagefocus = false;
    m_basicKeypad->update();
    //20200414 bug20294鼠标点击取消focus
    QPair<QString, Quantity> p;
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
        m_expressionBar->enterPercentEvent();
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
        if (m_keypadLayout->currentIndex() == 1) {
            m_keypadLayout->setCurrentIndex(0);
            MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mlist));
            btn5->setbtnlight(false);
        }
        break;
    case MemoryKeypadTab::Key_Mlist:
        showListWidget();
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
        p = m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber();
        m_expressionBar->getInputEdit()->setAnswer(p.first, p.second);
        this->handleClearStateChanged(false);
        break;
    default:
        break;
    }
    if (!pagefocus)
        m_expressionBar->getInputEdit()->setFocus();
}

/**
 * @brief 焦点空格事件
 * @param key-button枚举值
 */
void BasicModuleVer::handleKeypadButtonPressByspace(int key)
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
        m_expressionBar->enterPercentEvent();
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
        if (m_keypadLayout->currentIndex() == 1) {
            m_keypadLayout->setCurrentIndex(0);
            MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mlist));
            btn5->setbtnlight(false);
        }
        break;
    case MemoryKeypadTab::Key_Mlist:
        m_memoryKeypad->animate(MemoryKeypadTab::Key_Mlist, true);
        showListWidget();
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
        p = m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber();
        m_expressionBar->getInputEdit()->setAnswer(p.first, p.second);
        this->handleClearStateChanged(false);
        break;
    default:
        break;
    }
}

/**
 * @brief 设置清楚按键状态
 */
void BasicModuleVer::handleClearStateChanged(bool isAllClear)
{
    TextButton *btn = static_cast<TextButton *>(m_basicKeypad->button(BasicKeypadTab::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

void BasicModuleVer::setBasicTabOrder()
{
    this->setTabOrder(m_expressionBar->getInputEdit(), m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
    for (int i = 0; i < 5; i++) {
        this->setTabOrder(m_memoryKeypad->button(MemoryKeypadTab::Key_MC + i), m_memoryKeypad->button(MemoryKeypadTab::Key_MC + i + 1));
    }
    this->setTabOrder(m_memoryKeypad->button(MemoryKeypadTab::Key_Mlist), m_basicKeypad->button(BasicKeypadTab::Key_Clear));
    for (int i = 0; i < 20; i++) {
        this->setTabOrder(m_basicKeypad->button(BasicKeypadTab::Key_Clear + i), m_basicKeypad->button(BasicKeypadTab::Key_Clear + i + 1));
    }
}

/**
 * @brief mainwindow焦点不在basicmodul时也触发keypress
 */
void BasicModuleVer::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

/**
 * @brief 向内存中存入数据时触发
 */
void BasicModuleVer::mAvailableEvent()
{
    m_avail = true;
    MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
    btn->setEnabled(true);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MR));
    btn1->setEnabled(true);
    MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mlist));
    btn2->setEnabled(true);
    m_memRCbtn = true;
}

/**
 * @brief 清空内存列表触发
 */
void BasicModuleVer::mUnAvailableEvent()
{
    m_avail = false;
    MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
    btn->setEnabled(false);
    btn->updateWhenBtnDisable();
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MR));
    btn1->setEnabled(false);
    m_memRCbtn = false;
    if (m_keypadLayout->currentIndex() == 0) {
        MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mlist));
        btn2->setEnabled(false);
    }
    emit clearbtnShow(false);
}

/**
 * @brief 显示内存列表
 */
void BasicModuleVer::showListWidget()
{
    if (m_keypadLayout->currentIndex() == 0) {
        m_keypadLayout->setCurrentIndex(1);
    }
}

void BasicModuleVer::closeListWidget()
{
    //内存界面显示时，点击内存界面以外部分切换内存界面为键盘界面
    if (m_keypadLayout->currentIndex() == 1) {
        m_keypadLayout->setCurrentIndex(0);
        m_expressionBar->getInputEdit()->setFocus();
    }

    if (m_avail == true) {
        MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
        btn->setEnabled(true);
        MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MR));
        btn4->setEnabled(true);
        m_memRCbtn = true;
    } else {
        MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MC));
        btn->setEnabled(false);
        btn->updateWhenBtnDisable();
        MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_MR));
        btn1->setEnabled(false);
        MemoryButton *btn5 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypadTab::Key_Mlist));
        btn5->setEnabled(false);
        m_memRCbtn = false;
    }
    m_expressionBar->getInputEdit()->isExpressionEmpty(); //确认输入栏是否有内容，发送信号M+,M-,MS是否置灰
}

void BasicModuleVer::titleBarClean()
{
    m_memoryPublic->memoryclean();
    m_expressionBar->getInputEdit()->setFocus();
}

void BasicModuleVer::resetWindowSize(QSize size)
{
    m_basicKeypad->resetWidgetSize(size);
    m_memoryKeypad->resetWidgetSize(size);
    m_scihiswidget->resetWidgetSize(size);
    m_expressionBar->resetWidgetSize(size);
}

void BasicModuleVer::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void BasicModuleVer::checkLineEmpty()
{
    m_expressionBar->getInputEdit()->isExpressionEmpty();
    m_scihiswidget->resetFocus();
}
