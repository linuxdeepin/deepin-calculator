#include "scientificmodulever.h"

const int EXPRESSIONBAR_HEIGHT = 276;

scientificModuleVer::scientificModuleVer(QWidget *parent)
    : DWidget(parent)
{
    m_stackWidget = new QStackedWidget(this);
    m_sciexpressionBar = new SciExpressionBarTab(this);
    m_memhiskeypad = new MemHisKeypadTab(this);
    m_scikeypadwidget = new ScientificKeyPadTab(this);
    m_scihiswidget = new SciHistoryWidgetTab(3, this);
    m_memCalbtn = false;
    m_memRCbtn = false;
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_memoryPublic = MemoryPublicTab::instance();
    m_sciexpressionBar->setFixedHeight(EXPRESSIONBAR_HEIGHT);
    layout->addWidget(m_sciexpressionBar);
    layout->addWidget(m_memhiskeypad);
    m_stackWidget->addWidget(m_scihiswidget);
    m_stackWidget->addWidget(m_scikeypadwidget);
    m_stackWidget->setCurrentWidget(m_scikeypadwidget);
    layout->addWidget(m_stackWidget);
    layout->setSpacing(0);
    layout->setSpacing(0);
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
            &scientificModuleVer::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scikeypadwidget, &ScientificKeyPadTab::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scihiswidget, &SciHistoryWidgetTab::themeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_memoryPublic, &MemoryPublicTab::setThemeType);
    connect(m_sciexpressionBar, &SciExpressionBarTab::keyPress, this, &scientificModuleVer::handleEditKeyPress);
    connect(m_sciexpressionBar, &SciExpressionBarTab::clearStateChanged, this,
            &scientificModuleVer::handleClearStateChanged); //判断C/AC
    connect(m_sciexpressionBar, &SciExpressionBarTab::turnDeg, this, &scientificModuleVer::handleDegChanged); //点击deg按键
    connect(m_sciexpressionBar, &SciExpressionBarTab::fEStateChanged, this, &scientificModuleVer::handleFEStateChanged);
    connect(this, &scientificModuleVer::changedeg, m_scikeypadwidget, &ScientificKeyPadTab::getdeg); //切换deg切图
    connect(m_scikeypadwidget, &ScientificKeyPadTab::buttonPressed, this,
            &scientificModuleVer::handleKeypadButtonPress);
    connect(m_scikeypadwidget, &ScientificKeyPadTab::moveLeft, [ = ] { m_sciexpressionBar->moveLeft(); });
    connect(m_scikeypadwidget, &ScientificKeyPadTab::moveRight, [ = ] { m_sciexpressionBar->moveRight(); });
    connect(m_scikeypadwidget, &ScientificKeyPadTab::buttonPressedbySpace, this, &scientificModuleVer::handleKeypadButtonPressByspace);
    connect(m_memhiskeypad, &MemHisKeypadTab::buttonPressed, this,
            &scientificModuleVer::handleKeypadButtonPress);
    connect(m_memhiskeypad, &MemHisKeypadTab::moveLeft, [ = ] { m_sciexpressionBar->moveLeft(); });
    connect(m_memhiskeypad, &MemHisKeypadTab::moveRight, [ = ] { m_sciexpressionBar->moveRight(); });
    connect(m_memhiskeypad, &MemHisKeypadTab::buttonPressedbySpace, this, &scientificModuleVer::handleKeypadButtonPressByspace);
    connect(m_scihiswidget, &SciHistoryWidgetTab::hisIsFilled, [ = ](bool hisisfilled) {
        if (hisisfilled) {
            MemoryButton *btn = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
            btn->setEnabled(true);
            m_havail = true;
        } else {
            m_havail = false;
        }
    });
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetplus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetplus(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
    });
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::widgetminus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_memoryPublic->widgetminus(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
    });

    connect(m_memoryPublic, &MemoryPublicTab::memorycleanSig, this, &scientificModuleVer::mUnAvailableEvent);
    connect(m_memoryPublic, &MemoryPublicTab::generateDataSig, this, &scientificModuleVer::mAvailableEvent);
    connect(m_scihiswidget->findChild<MemoryWidgetTab *>(), &MemoryWidgetTab::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
        QString str = p.first;
        m_sciexpressionBar->getInputEdit()->setAnswer(str.remove("\n"), p.second);
        m_sciexpressionBar->getInputEdit()->setFocus();
        //点击item清除键状态改变
        this->handleClearStateChanged(false);
        m_stackWidget->setCurrentWidget(m_scikeypadwidget);
    });
    connect(m_sciexpressionBar->getInputEdit(), &InputEdit::emptyExpression, this, [ = ](bool b) {
        if (b == false) {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mplus));
            btn2->setEnabled(true);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mmin));
            btn3->setEnabled(true);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MS));
            btn4->setEnabled(true);
            m_memCalbtn = true;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
        } else {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_Mmin));
            btn3->setEnabled(false);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MS));
            btn4->setEnabled(false);
            m_memCalbtn = false;
            m_scihiswidget->findChild<MemoryWidgetTab *>()->expressionempty(b);
        }
    });

    //titlebar的清除按钮
    connect(m_scihiswidget, &SciHistoryWidgetTab::clearbtnShow, this, &scientificModuleVer::clearbtnShow);
    if (!m_memoryPublic->isEmpty())
        mAvailableEvent();
    else
        mUnAvailableEvent();
//    setScientificTabOrder();
}

scientificModuleVer::~scientificModuleVer()
{

}

void scientificModuleVer::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

void scientificModuleVer::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void scientificModuleVer::checkLineEmpty()
{
    m_sciexpressionBar->getInputEdit()->isExpressionEmpty();
    m_scihiswidget->resetFocus();
}

void scientificModuleVer::mAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
    btn->setEnabled(true);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
    btn1->setEnabled(true);
    MemoryButton *btn2 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
    btn2->setEnabled(true);
    m_memRCbtn = true;
    m_avail = true;
}

void scientificModuleVer::mUnAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
    btn->setEnabled(false);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
    btn1->setEnabled(false);
    if (m_stackWidget->currentWidget() == m_scikeypadwidget && m_havail == false) {
        MemoryButton *btn2 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
        btn2->setEnabled(false);
    }
    m_memRCbtn = false;
    m_avail = false;
}

void scientificModuleVer::showListWidget()
{
    m_stackWidget->setCurrentWidget(m_scihiswidget);
    if (static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist))->hasFocus()) {
        m_scihiswidget->findChild<DButtonBoxButton *>("mButtonBoxButton")->setFocus();
    }
}

void scientificModuleVer::closeListWidget()
{
    m_stackWidget->setCurrentWidget(m_scikeypadwidget);
    m_sciexpressionBar->getInputEdit()->setFocus();
    if (m_avail == true) {
        MemoryButton *btn = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
        btn->setbuttongray(false);
        btn->setEnabled(true);
        MemoryButton *btn7 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
        btn7->setbuttongray(false);
        btn7->setEnabled(true);
        m_memRCbtn = true;
    } else {
        MemoryButton *btn = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MC));
        btn->setbuttongray(true);
        btn->setEnabled(false);
        MemoryButton *btn1 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MR));
        btn->setbuttongray(true);
        btn1->setEnabled(false);
        if (m_havail == false) {
            MemoryButton *btn8 = static_cast<MemoryButton *>(m_memhiskeypad->button(MemHisKeypadTab::Key_MHlist));
            btn8->setEnabled(false);
        }
        m_memRCbtn = false;
    }
    m_sciexpressionBar->getInputEdit()->isExpressionEmpty(); //确认输入栏是否有内容，发送信号M+,M-,MS是否置灰
}

void scientificModuleVer::titleBarClean()
{
    m_scihiswidget->cleanButtonEvent();
    m_sciexpressionBar->getInputEdit()->setFocus();
}

void scientificModuleVer::resetWindowSize(QSize size)
{
    m_scikeypadwidget->resetWidgetSize(size);
    m_memhiskeypad->resetWidgetSize(size);
    m_scihiswidget->resetWidgetSize(size);
    m_sciexpressionBar->setFixedHeight(EXPRESSIONBAR_HEIGHT * size.height() / 1880);
    m_sciexpressionBar->resetWidgetSize(size);
}

void scientificModuleVer::initTheme(int type)
{
    m_sciexpressionBar->initTheme(type);
    update();
}

void scientificModuleVer::handleEditKeyPress(QKeyEvent *e)
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
            m_memhiskeypad->animate(MemHisKeypadTab::Key_FE);
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
            m_memhiskeypad->animate(MemHisKeypadTab::Key_MC);
            QTimer::singleShot(100, this, [ = ] {
//                m_memorylistwidget->memoryclean();
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
            m_memhiskeypad->animate(MemHisKeypadTab::Key_MR);
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
            m_memhiskeypad->animate(MemHisKeypadTab::Key_Mplus);
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
            m_memhiskeypad->animate(MemHisKeypadTab::Key_Mmin);
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
            m_memhiskeypad->animate(MemHisKeypadTab::Key_MS);
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

void scientificModuleVer::handleKeypadButtonPress(int key)
{
    m_scikeypadwidget->update();
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
    case MemHisKeypadTab::Key_MS:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::generateData, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemHisKeypadTab::Key_MC:
        m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryclean);
        break;
    case MemHisKeypadTab::Key_Mplus:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemHisKeypadTab::Key_Mmin:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemHisKeypadTab::Key_MR:
        m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                      , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
        this->handleClearStateChanged(false);
        break;
    case MemHisKeypadTab::Key_MHlist:
        showListWidget();
        break;
    case ScientificKeyPadTab::Key_deg:
        m_sciexpressionBar->enterDegEvent(m_deg);
        break;
    case ScientificKeyPadTab::Key_sin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSinEvent();
        break;
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
}

void scientificModuleVer::handleKeypadButtonPressByspace(int key)
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
    case MemHisKeypadTab::Key_MS:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::generateData, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemHisKeypadTab::Key_MC:
        m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryclean);
        break;
    case MemHisKeypadTab::Key_Mplus:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryplus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemHisKeypadTab::Key_Mmin:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(SciHistoryWidgetTab::memoryminus, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        break;
    case MemHisKeypadTab::Key_MR:
        m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().first
                                                      , m_scihiswidget->findChild<MemoryWidgetTab *>()->getfirstnumber().second);
        this->handleClearStateChanged(false);
        break;
    case MemHisKeypadTab::Key_MHlist:
        showListWidget();
        break;
    case ScientificKeyPadTab::Key_deg:
        m_sciexpressionBar->enterDegEvent(m_deg);
        break;
    case ScientificKeyPadTab::Key_sin:
        if (!m_sciexpressionBar->judgeinput())
            return;
        m_sciexpressionBar->enterSinEvent();
        break;
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

void scientificModuleVer::handleClearStateChanged(bool isAllClear)
{
    TextButtonTab *btn = static_cast<TextButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

void scientificModuleVer::handleDegChanged()
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

void scientificModuleVer::handleFEStateChanged(bool isdown)
{
    TextButtonTab *btn = static_cast<TextButtonTab *>(m_memhiskeypad->button(MemHisKeypadTab::Key_FE));
    if (isdown) {
        m_FEisdown = true;
    } else {
        m_FEisdown = false;
    }
    btn->setButtonDown(m_FEisdown);
}

void scientificModuleVer::handlePageStateChanged()
{
    TextButtonTab *btn = static_cast<TextButtonTab *>(m_scikeypadwidget->button(ScientificKeyPadTab::Key_page));
    if (m_Pageisdown) {
        m_Pageisdown = false;
    } else {
        m_Pageisdown = true;
    }
    btn->setButtonDown(m_Pageisdown);
}
