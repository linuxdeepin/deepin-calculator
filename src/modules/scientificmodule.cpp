#include "scientificmodule.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "dthememanager.h"
#include "../utils.h"
#include "../math/quantity.h"

scientificModule::scientificModule(QWidget *parent)
    : DWidget(parent)
{
//    m_scikeypadwidget = new ScientificKeyPad;
    m_scikeypadwidget = new ScientificKeyPad;
    m_insidewidget = false;
    m_scihiswidget = new SciHistoryWidget(this);
    m_memCalbtn = false;
    m_memRCbtn = false;
//    m_keypadLayout->addWidget(m_basicKeypad);
    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *leftlay = new QVBoxLayout();
    m_sciexpressionBar = new SciExpressionBar;
    leftlay->addWidget(m_sciexpressionBar);
//    leftlay->addWidget(m_scikeypadwidget);
//    m_triCombobox->setFixedSize(60, 40);
//    layout->addWidget(m_triCombobox);
    leftlay->addWidget(m_scikeypadwidget);

    leftlay->setSpacing(0);
//    leftlay->setMargin(0);
//    leftlay->setContentsMargins(0, 0, 0, 11);

    QVBoxLayout *rightlay = new QVBoxLayout();
    rightlay->addWidget(m_scihiswidget);
    layout->addLayout(leftlay);
    layout->addLayout(rightlay);
    this->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 11);

    setMouseTracking(true);
    setFocus();

    // initTheme();
    m_sciexpressionBar->initTheme(DGuiApplicationHelper::instance()->themeType());
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_sciexpressionBar, &SciExpressionBar::initTheme);

    DPalette pal = this->palette();
    pal.setColor(DPalette::Light, QColor(248, 248, 248));
    this->setPalette(pal);

    connect(m_scihiswidget->m_listDelegate, &SimpleListDelegate::obtainingHistorical, m_sciexpressionBar,
            &SciExpressionBar::revisionResults);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged, this,
            &scientificModule::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scikeypadwidget, &ScientificKeyPad::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scihiswidget, &SciHistoryWidget::themeChanged);
    connect(m_sciexpressionBar, &SciExpressionBar::keyPress, this, &scientificModule::handleEditKeyPress);
    connect(m_sciexpressionBar, &SciExpressionBar::clearStateChanged, this,
            &scientificModule::handleClearStateChanged);
    connect(m_sciexpressionBar, &SciExpressionBar::turnDeg, this, &scientificModule::handleDegChanged);
    connect(m_sciexpressionBar, &SciExpressionBar::fEStateChanged, this, &scientificModule::handleFEStateChanged);
    connect(this, &scientificModule::changedeg, m_scikeypadwidget, &ScientificKeyPad::getdeg);
//    connect(m_basicKeypad, &BasicKeypad::buttonPressed, this,
//            &scientificModule::handleKeypadButtonPress);
//    connect(m_basicKeypad, &BasicKeypad::equalPressed, this, &scientificModule::equalButtonPress);
//    connect(m_basicKeypad, &BasicKeypad::moveLeft, [ = ] { m_sciexpressionBar->moveLeft(); });
//    connect(m_basicKeypad, &BasicKeypad::moveRight, [ = ] { m_sciexpressionBar->moveRight(); });
    connect(m_scikeypadwidget, &ScientificKeyPad::buttonPressed, this,
            &scientificModule::handleKeypadButtonPress);
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::widgetplus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::widgetplus);
    });
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::widgetminus, this, [ = ](int row) {
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
            m_scihiswidget->memoryFunctions(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::widgetminus);
    });
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::mListUnavailable, this, &scientificModule::mUnAvailableEvent);
    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::mListAvailable, this, &scientificModule::mAvailableEvent);

    connect(m_scihiswidget->getMemoryWidget(), &MemoryWidget::itemclick, this, [ = ](const QPair<QString, Quantity> p) {
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
    connect(m_scikeypadwidget, &ScientificKeyPad::funshow, this, [ = ]() {
        m_sciexpressionBar->setAttribute(Qt::WA_TransparentForMouseEvents);
    });
    connect(m_scikeypadwidget, &ScientificKeyPad::funinside, this, [ = ]() {
        m_isinsidefun = true;
    });
    connect(m_sciexpressionBar->getInputEdit(), &InputEdit::emptyExpression, this, [ = ](bool b) {
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
    // m_expBarColor = "#F8F8F8";
    // m_expBarSepColor = "#F8F8F8";
}

scientificModule::~scientificModule() {}

void scientificModule::initTheme(int type)
{
    m_sciexpressionBar->initTheme(type);
    update();
}

void scientificModule::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;
    const QString keyText = e->text();
    bool lineFocus = m_sciexpressionBar->getInputEdit()->hasFocus();
    //20200414 bug20294鼠标点击取消focus
//    m_sciexpressionBar->getInputEdit()->setFocus();
    // m_sciexpressionBar->clearSelection();

    switch (e->key()) {
    case Qt::Key_0:
        m_sciexpressionBar->enterNumberEvent("0");
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_1:
        m_sciexpressionBar->enterNumberEvent("1");
        // m_basicKeypad->button(BasicKeypad::Key_1)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_2:
        m_sciexpressionBar->enterNumberEvent("2");
        // m_basicKeypad->button(BasicKeypad::Key_2)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_3:
        m_sciexpressionBar->enterNumberEvent("3");
        // m_basicKeypad->button(BasicKeypad::Key_3)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_4:
        m_sciexpressionBar->enterNumberEvent("4");
        // m_basicKeypad->button(BasicKeypad::Key_4)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_5:
        m_sciexpressionBar->enterNumberEvent("5");
        // m_basicKeypad->button(BasicKeypad::Key_5)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_6:
        m_sciexpressionBar->enterNumberEvent("6");
        // m_basicKeypad->button(BasicKeypad::Key_6)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_7:
        m_sciexpressionBar->enterNumberEvent("7");
        // m_basicKeypad->button(BasicKeypad::Key_7)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_8:
        m_sciexpressionBar->enterNumberEvent("8");
        // m_basicKeypad->button(BasicKeypad::Key_8)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_9:
        m_sciexpressionBar->enterNumberEvent("9");
        // m_basicKeypad->button(BasicKeypad::Key_9)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Plus:
        m_sciexpressionBar->enterSymbolEvent("+");
        // m_basicKeypad->button(BasicKeypad::Key_Plus)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_sciexpressionBar->enterSymbolEvent("-");
        // m_basicKeypad->button(BasicKeypad::Key_Min)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Asterisk:
        m_sciexpressionBar->enterSymbolEvent("*");
        // m_basicKeypad->button(BasicKeypad::Key_Mult)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Slash:
        m_sciexpressionBar->enterSymbolEvent("/");
        // m_basicKeypad->button(BasicKeypad::Key_Div)->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        //(dynamic_cast<DPushButton
        //*>(m_basicKeypad->button(BasicKeypad::Key_Equals)))->animateClick();
        // m_basicKeypad->button()->animateClick();
//        m_sciexpressionBar->settingLinkage();
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getexpression().first) {
            m_scihiswidget->m_listModel->updataList(m_sciexpressionBar->getexpression().second, -2, true);
            m_scihiswidget->ishistoryfilled(true);
        }
        m_sciexpressionBar->addUndo();
        setFocus();
        break;
    case Qt::Key_Backspace:
        m_sciexpressionBar->enterBackspaceEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Period:
        m_sciexpressionBar->enterPointEvent();
        //(static_cast<DPushButton
        //*>(m_basicKeypad->button(BasicKeypad::Key_Point)))->animateClick();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Escape:
        //            (static_cast<DPushButton
        //            *>(m_basicKeypad->button(BasicKeypad::Key_Clear)))
        //                ->animateClick();
        m_sciexpressionBar->enterClearEvent();
        //            m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_ParenLeft:
        // m_sciexpressionBar->setContinue(true);
        // m_sciexpressionBar->enterBracketsEvent();
        // m_basicKeypad->button(BasicKeypad::Key_Brackets)->animateClick();
        m_sciexpressionBar->enterLeftBracketsEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_ParenRight:
        m_sciexpressionBar->enterRightBracketsEvent();
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Percent:
        m_sciexpressionBar->enterPercentEvent();
        // m_basicKeypad->button(BasicKeypad::Key_Percent)->animateClick();
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
            m_sciexpressionBar->enterSymbolEvent("*");
            m_sciexpressionBar->addUndo();
        }
        m_sciexpressionBar->addUndo();
        break;
    case Qt::Key_Delete:
        m_sciexpressionBar->enterClearEvent();
        //            m_sciexpressionBar->addUndo();
        // m_sciexpressionBar->enterDeleteEvent();
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
        break;
    case Qt::Key_L:
        if (isPressCtrl && m_memRCbtn) {
//            m_scikeypadwidget->animate(MemoryKeypad::Key_MC);
            QTimer::singleShot(100, this, [ = ] {
//                m_memorylistwidget->memoryclean();
                m_scihiswidget->memoryFunctions(-1, Quantity(), SciHistoryWidget::memoryclean);
            });
        }
        break;
    case Qt::Key_R:
        if (isPressCtrl && m_memRCbtn) {
//            m_memoryKeypad->animate(MemoryKeypad::Key_MR);
            m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->getMemoryWidget()->getfirstnumber().first
                                                          , m_scihiswidget->getMemoryWidget()->getfirstnumber().second);
        }
        break;
    case Qt::Key_P:
        if (isPressCtrl && m_memCalbtn) {
//            m_memoryKeypad->animate(MemoryKeypad::Key_Mplus);
            m_sciexpressionBar->enterEqualEvent();
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
                m_scihiswidget->memoryFunctions(-1, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::memoryplus);
//                memoryPublic->memoryplus(m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    case Qt::Key_Q:
        if (isPressCtrl && m_memCalbtn) {
//            m_memoryKeypad->animate(MemoryKeypad::Key_Mminus);
            m_sciexpressionBar->enterEqualEvent();
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
                m_scihiswidget->memoryFunctions(-1, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::memoryminus);
//                memoryPublic->memoryminus(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    case Qt::Key_M:
        if (isPressCtrl && m_memCalbtn) {
//            m_memoryKeypad->animate(MemoryKeypad::Key_MS);
            m_sciexpressionBar->enterEqualEvent();
            if (m_sciexpressionBar->getInputEdit()->getMemoryAnswer().first)
                m_scihiswidget->memoryFunctions(-1, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::generateData);
//                memoryPublic->generateData(m_expressionBar->getInputEdit()->getMemoryAnswer().second);
        }
        break;
    /*case Qt::Key_E:
        m_sciexpressionBar->entereEvent();
        m_sciexpressionBar->addUndo();
        breal;*/
    default:
        break;
    }
    m_sciexpressionBar->getInputEdit()->setFocus(); //edit 20200417 for bug--21146
    // m_sciexpressionBar->setSelection();
//    if (lineFocus)
//        m_sciexpressionBar->getInputEdit()->setFocus();
//    else
//        setFocus();
}

void scientificModule::handleKeypadButtonPress(int key)
{
//    m_basicKeypad->update();
    m_scikeypadwidget->update();
    m_sciexpressionBar->clearSelection();
    //20200414 bug20294鼠标点击取消focus
    m_sciexpressionBar->getInputEdit()->setFocus();
    switch (key) {
    case ScientificKeyPad::Key_0:
        m_sciexpressionBar->enterNumberEvent("0");
        break;
    case ScientificKeyPad::Key_1:
        m_sciexpressionBar->enterNumberEvent("1");
        break;
    case ScientificKeyPad::Key_2:
        m_sciexpressionBar->enterNumberEvent("2");
        break;
    case ScientificKeyPad::Key_3:
        m_sciexpressionBar->enterNumberEvent("3");
        break;
    case ScientificKeyPad::Key_4:
        m_sciexpressionBar->enterNumberEvent("4");
        break;
    case ScientificKeyPad::Key_5:
        m_sciexpressionBar->enterNumberEvent("5");
        break;
    case ScientificKeyPad::Key_6:
        m_sciexpressionBar->enterNumberEvent("6");
        break;
    case ScientificKeyPad::Key_7:
        m_sciexpressionBar->enterNumberEvent("7");
        break;
    case ScientificKeyPad::Key_8:
        m_sciexpressionBar->enterNumberEvent("8");
        break;
    case ScientificKeyPad::Key_9:
        m_sciexpressionBar->enterNumberEvent("9");
        break;
    case ScientificKeyPad::Key_Plus:
        m_sciexpressionBar->enterSymbolEvent("+");
        break;
    case ScientificKeyPad::Key_Min:
        m_sciexpressionBar->enterSymbolEvent("-");
        break;
    case ScientificKeyPad::Key_Mult:
        m_sciexpressionBar->enterSymbolEvent("*");
        break;
    case ScientificKeyPad::Key_Div:
        m_sciexpressionBar->enterSymbolEvent("/");
        break;
    case ScientificKeyPad::Key_Percent:
        m_sciexpressionBar->enterPercentEvent();
        break;
    case ScientificKeyPad::Key_Equals:
        m_sciexpressionBar->enterEqualEvent();
        if (m_sciexpressionBar->getexpression().first) {
            m_scihiswidget->m_listModel->updataList(m_sciexpressionBar->getexpression().second, -2, true);
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
        m_sciexpressionBar->enterPointEvent();
        break;
    case ScientificKeyPad::Key_left:
        m_sciexpressionBar->enterLeftBracketsEvent();
        break;
    case ScientificKeyPad::Key_right:
        m_sciexpressionBar->enterRightBracketsEvent();
        break;
    case ScientificKeyPad::Key_MS:
        m_sciexpressionBar->enterEqualEvent();
        m_scihiswidget->memoryFunctions(-1, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::generateData);
        break;
    case ScientificKeyPad::Key_MC:
        m_scihiswidget->memoryFunctions(-1, Quantity(), SciHistoryWidget::memoryclean);
        break;
//    case ScientificKeyPad::Key_Mlist:
//        showListWidget();
//        if (m_keypadLayout->currentIndex() == 1) {
//            m_sciexpressionBar->setAttribute(Qt::WA_TransparentForMouseEvents);
//            m_scikeypadwidget->setAttribute(Qt::WA_TransparentForMouseEvents);
//        } else {
//            m_sciexpressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//            m_scikeypadwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//        }
//        m_memorylistwidget->setFocus();
//        break;
    case ScientificKeyPad::Key_Mplus:
        m_sciexpressionBar->enterEqualEvent();
        m_scihiswidget->memoryFunctions(-1, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::memoryplus);
        break;
    case ScientificKeyPad::Key_Mmin:
        m_sciexpressionBar->enterEqualEvent();
        m_scihiswidget->memoryFunctions(-1, m_sciexpressionBar->getInputEdit()->getMemoryAnswer().second, SciHistoryWidget::memoryminus);
        break;
    case ScientificKeyPad::Key_MR:
        m_sciexpressionBar->getInputEdit()->setAnswer(m_scihiswidget->getMemoryWidget()->getfirstnumber().first
                                                      , m_scihiswidget->getMemoryWidget()->getfirstnumber().second);
        break;
    case ScientificKeyPad::Key_deg:
        m_sciexpressionBar->enterDegEvent(m_deg);
        break;
    case ScientificKeyPad::Key_sin:
        m_sciexpressionBar->enterSinEvent();
        break;
    case ScientificKeyPad::Key_FE:
        m_sciexpressionBar->enterFEEvent(m_FEisdown);
        break;
    case ScientificKeyPad::Key_page:
        handlePageStateChanged();
        break;
    case ScientificKeyPad::Key_PI:
        m_sciexpressionBar->enterPIEvent();
        break;
    case ScientificKeyPad::Key_e:
        m_sciexpressionBar->enterEulerEvent();
        break;
    case ScientificKeyPad::Key_Mod:
        m_sciexpressionBar->enterModEvent();
        break;
    case ScientificKeyPad::Key_x2:
        m_sciexpressionBar->enterx2Event();
        break;
    case ScientificKeyPad::Key_Derivative:
        m_sciexpressionBar->enterDerivativeEvent();
        break;
    case ScientificKeyPad::Key_Factorials:
        m_sciexpressionBar->enterFactorialsEvent();
        break;
    case ScientificKeyPad::Key_exp:
        m_sciexpressionBar->enterExpEvent();
        break;
    case ScientificKeyPad::Key_cos:
        m_sciexpressionBar->enterCosEvent();
        break;
    case ScientificKeyPad::Key_x3:
        m_sciexpressionBar->enterx3Event();
        break;
    case ScientificKeyPad::Key_tan:
        m_sciexpressionBar->enterTanEvent();
        break;
    case ScientificKeyPad::Key_xy:
        m_sciexpressionBar->enterxyEvent();
        break;
    case ScientificKeyPad::Key_cot:
        m_sciexpressionBar->enterCotEvent();
        break;
    case ScientificKeyPad::Key_10x:
        m_sciexpressionBar->enter10xEvent();
        break;
    case ScientificKeyPad::Key_Modulus:
        m_sciexpressionBar->enterModulusEvent();
        break;
    case ScientificKeyPad::Key_log:
        m_sciexpressionBar->enterLogEvent();
        break;
    case ScientificKeyPad::Key_Rand:
        m_sciexpressionBar->enterRandEvent();
        break;
    case ScientificKeyPad::Key_ln:
        m_sciexpressionBar->enterLnEvent();
        break;
    case ScientificKeyPad::Key_arcsin:
        m_sciexpressionBar->enterArcsinEvent();
        break;
    case ScientificKeyPad::Key_sqrt2:
        m_sciexpressionBar->enterSqrtEvent();
        break;
    case ScientificKeyPad::Key_arccos:
        m_sciexpressionBar->enterArccosEvent();
        break;
    case ScientificKeyPad::Key_sqrt3:
        m_sciexpressionBar->enterCbrtEvent();
        break;
    case ScientificKeyPad::Key_arctan:
        m_sciexpressionBar->enterArctanEvent();
        break;
    case ScientificKeyPad::Key_ysqrtx:
        m_sciexpressionBar->enterYrootEvent();
        break;
    case ScientificKeyPad::Key_arccot:
        m_sciexpressionBar->enterArccotEvent();
        break;
    case ScientificKeyPad::Key_2x:
        m_sciexpressionBar->enter2xEvent();
        break;
    case ScientificKeyPad::Key_logyx:
        m_sciexpressionBar->enterlogyxEvent();
        break;
    case ScientificKeyPad::Key_ex:
        m_sciexpressionBar->enterexEvent();
        break;
    case ScientificKeyPad::Key_opposite:
        m_sciexpressionBar->enterOppositeEvent();
        break;
    }
    m_sciexpressionBar->addUndo();
}

void scientificModule::equalButtonPress()
{
//    m_sciexpressionBar->settingLinkage();
    m_sciexpressionBar->enterEqualEvent();
    m_sciexpressionBar->addUndo();
}

void scientificModule::handleClearStateChanged(bool isAllClear)
{
    TextButton *btn = static_cast<TextButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Clear));

    if (isAllClear) {
        btn->setText("AC");
    } else {
        btn->setText("C");
    }
}

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

/*void scientificModule::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);

    // draw expression bar background;
    painter.setBrush(m_expBarColor);
    painter.drawRect(QRect(0, 0, rect().width(), m_sciexpressionBar->height() - 1));

    // draw expression bar bottom separator.
    painter.setBrush(m_expBarSepColor);
    painter.drawRect(QRect(0, m_sciexpressionBar->height(), rect().width(), 1));

    // draw buttons separator.
    painter.setBrush(m_btnSepColor);
    painter.drawRect(QRect(rect().x(),
                           m_sciexpressionBar->height() + 1,
                           rect().width(),
                           rect().height()));
}*/

void scientificModule::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

void scientificModule::mAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MC));
    btn->setEnabled(true);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MR));
    btn1->setEnabled(true);
    m_memRCbtn = true;
    m_avail = true;
}

void scientificModule::mUnAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MC));
    btn->setEnabled(false);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MR));
    btn1->setEnabled(false);
    m_memRCbtn = false;
    m_avail = false;
}

void scientificModule::mousePressEvent(QMouseEvent *event)
{
    if (m_isinsidefun == false) {
        m_sciexpressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_scikeypadwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
    m_insidewidget = false;
    m_sciexpressionBar->getInputEdit()->isExpressionEmpty();
    QWidget::mousePressEvent(event);
}

void scientificModule::showOrHideHistory(bool b)
{
    if (b == true)
        m_scihiswidget->hide();
    else {
        m_scihiswidget->show();
        m_scihiswidget->focusOnButtonbox();
    }
}

void scientificModule::checkLineEmpty()
{
    m_sciexpressionBar->getInputEdit()->isExpressionEmpty();
}

