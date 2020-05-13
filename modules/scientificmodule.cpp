#include "scientificmodule.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "dthememanager.h"
#include "utils.h"
#include "../math/quantity.h"

scientificModule::scientificModule(QWidget *parent)
    : DWidget(parent)
{
    m_memoryKeypad = new MemoryKeypad;
    m_scikeypadwidget = new SciBasicKeyPad;
    m_insidewidget = false;
    m_scihiswidget = new SciHistoryWidget;
//    m_keypadLayout->addWidget(m_basicKeypad);
    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *leftlay = new QVBoxLayout();
    m_expressionBar = new ExpressionBar;
    leftlay->addWidget(m_expressionBar);
    leftlay->addWidget(m_memoryKeypad);
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
    m_expressionBar->initTheme(DGuiApplicationHelper::instance()->themeType());
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_expressionBar, &ExpressionBar::initTheme);

    DPalette pal = this->palette();
    pal.setColor(DPalette::Light, QColor(248, 248, 248));
    this->setPalette(pal);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged, this,
            &scientificModule::initTheme);
//    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
//            m_basicKeypad, &BasicKeypad::buttonThemeChanged);
    connect(m_expressionBar, &ExpressionBar::keyPress, this, &scientificModule::handleEditKeyPress);
    connect(m_expressionBar, &ExpressionBar::clearStateChanged, this,
            &scientificModule::handleClearStateChanged);
    connect(m_expressionBar, &ExpressionBar::turnDeg, this, &scientificModule::handleDegChanged);
//    connect(m_basicKeypad, &BasicKeypad::buttonPressed, this,
//            &scientificModule::handleKeypadButtonPress);
//    connect(m_basicKeypad, &BasicKeypad::equalPressed, this, &scientificModule::equalButtonPress);
//    connect(m_basicKeypad, &BasicKeypad::moveLeft, [ = ] { m_expressionBar->moveLeft(); });
//    connect(m_basicKeypad, &BasicKeypad::moveRight, [ = ] { m_expressionBar->moveRight(); });
    connect(m_memoryKeypad, &MemoryKeypad::buttonPressed, this,
            &scientificModule::handleKeypadButtonPress);
    connect(m_scikeypadwidget, &SciBasicKeyPad::buttonPressed, this,
            &scientificModule::handleKeypadButtonPress);
//    connect(m_memorylistwidget, &MemoryWidget::widgetplus, this, [ = ](int row) {
//        m_memorylistwidget->widgetplusslot(row, m_expressionBar->getInputEdit()->getMemoryAnswer());
//    });
//    connect(m_memorylistwidget, &MemoryWidget::widgetminus, this, [ = ](int row) {
//        m_memorylistwidget->widgetminusslot(row, m_expressionBar->getInputEdit()->getMemoryAnswer());
//    });
//    connect(m_memorylistwidget, &MemoryWidget::insidewidget, this, [ = ]() {
//        m_insidewidget = true;
//    });
//    connect(m_memorylistwidget, &MemoryWidget::mListUnavailable, this, [ = ]() {
//        mUnAvailableEvent();
//        m_avail = false;
//    });
//    connect(m_memorylistwidget, &MemoryWidget::mListAvailable, this, [ = ]() {
//        mAvailableEvent();
//        m_avail = true;
//    });
//    connect(m_memorylistwidget, &MemoryWidget::itemclick, this, [ = ](const QString str) {
//        m_expressionBar->getInputEdit()->setText(str);
//        m_expressionBar->getInputEdit()->setFocus();
//        if (m_keypadLayout->currentIndex() == 1) {
//            m_keypadLayout->setCurrentIndex(0);
//            m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//            m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//            MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
//            btn->setEnabled(true);
//            MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
//            btn1->setEnabled(true);
//            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
//            btn2->setEnabled(true);
//            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
//            btn3->setEnabled(true);
//            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
//            btn4->setEnabled(true);
//        }
//    });
    connect(m_scikeypadwidget, &SciBasicKeyPad::funshow, this, [ = ]() {
        m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents);
        m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents);
    });
    connect(m_scikeypadwidget, &SciBasicKeyPad::funinside, this, [ = ]() {
        m_isinsidefun = true;
    });
    connect(m_expressionBar->getInputEdit(), &InputEdit::correctExpression, this, [ = ](bool b) {
        if (b == true) {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
            btn2->setEnabled(true);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
            btn3->setEnabled(true);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
            btn4->setEnabled(true);
        } else {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mminus));
            btn3->setEnabled(false);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MS));
            btn4->setEnabled(false);
        }
    });
    // m_expBarColor = "#F8F8F8";
    // m_expBarSepColor = "#F8F8F8";
}

scientificModule::~scientificModule() {}

void scientificModule::initTheme(int type)
{
    m_expressionBar->initTheme(type);
    update();
}

void scientificModule::handleEditKeyPress(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;
    const QString keyText = e->text();
    bool lineFocus = m_expressionBar->getInputEdit()->hasFocus();
    //20200414 bug20294鼠标点击取消focus
//    m_expressionBar->getInputEdit()->setFocus();
    // m_expressionBar->clearSelection();

    switch (e->key()) {
    case Qt::Key_0:
        m_expressionBar->enterNumberEvent("0");
        m_expressionBar->addUndo();
        break;
    case Qt::Key_1:
        m_expressionBar->enterNumberEvent("1");
        // m_basicKeypad->button(BasicKeypad::Key_1)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_2:
        m_expressionBar->enterNumberEvent("2");
        // m_basicKeypad->button(BasicKeypad::Key_2)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_3:
        m_expressionBar->enterNumberEvent("3");
        // m_basicKeypad->button(BasicKeypad::Key_3)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_4:
        m_expressionBar->enterNumberEvent("4");
        // m_basicKeypad->button(BasicKeypad::Key_4)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_5:
        m_expressionBar->enterNumberEvent("5");
        // m_basicKeypad->button(BasicKeypad::Key_5)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_6:
        m_expressionBar->enterNumberEvent("6");
        // m_basicKeypad->button(BasicKeypad::Key_6)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_7:
        m_expressionBar->enterNumberEvent("7");
        // m_basicKeypad->button(BasicKeypad::Key_7)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_8:
        m_expressionBar->enterNumberEvent("8");
        // m_basicKeypad->button(BasicKeypad::Key_8)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_9:
        m_expressionBar->enterNumberEvent("9");
        // m_basicKeypad->button(BasicKeypad::Key_9)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Plus:
        m_expressionBar->enterSymbolEvent("+");
        // m_basicKeypad->button(BasicKeypad::Key_Plus)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
        m_expressionBar->enterSymbolEvent("-");
        // m_basicKeypad->button(BasicKeypad::Key_Min)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Asterisk:
        m_expressionBar->enterSymbolEvent("*");
        // m_basicKeypad->button(BasicKeypad::Key_Mult)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Slash:
        m_expressionBar->enterSymbolEvent("/");
        // m_basicKeypad->button(BasicKeypad::Key_Div)->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        //(dynamic_cast<DPushButton
        //*>(m_basicKeypad->button(BasicKeypad::Key_Equals)))->animateClick();
        // m_basicKeypad->button()->animateClick();
        m_expressionBar->settingLinkage();
        m_expressionBar->addUndo();
        setFocus();
        break;
    case Qt::Key_Backspace:
        m_expressionBar->enterBackspaceEvent();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Period:
        m_expressionBar->enterPointEvent();
        //(static_cast<DPushButton
        //*>(m_basicKeypad->button(BasicKeypad::Key_Point)))->animateClick();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Escape:
        //            (static_cast<DPushButton
        //            *>(m_basicKeypad->button(BasicKeypad::Key_Clear)))
        //                ->animateClick();
        m_expressionBar->enterClearEvent();
        //            m_expressionBar->addUndo();
        break;
    case Qt::Key_ParenLeft:
        // m_expressionBar->setContinue(true);
        // m_expressionBar->enterBracketsEvent();
        // m_basicKeypad->button(BasicKeypad::Key_Brackets)->animateClick();
        m_expressionBar->enterLeftBracketsEvent();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_ParenRight:
        m_expressionBar->enterRightBracketsEvent();
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Percent:
        m_expressionBar->enterPercentEvent();
        // m_basicKeypad->button(BasicKeypad::Key_Percent)->animateClick();
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
            m_expressionBar->addUndo();
        }
        m_expressionBar->addUndo();
        break;
    case Qt::Key_Delete:
        m_expressionBar->enterClearEvent();
        //            m_expressionBar->addUndo();
        // m_expressionBar->enterDeleteEvent();
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
    /*case Qt::Key_E:
        m_expressionBar->entereEvent();
        m_expressionBar->addUndo();
        breal;*/
    default:
        break;
    }
    m_expressionBar->getInputEdit()->setFocus(); //edit 20200417 for bug--21146
    // m_expressionBar->setSelection();
//    if (lineFocus)
//        m_expressionBar->getInputEdit()->setFocus();
//    else
//        setFocus();
}

void scientificModule::handleKeypadButtonPress(int key)
{
//    m_basicKeypad->update();
    m_scikeypadwidget->update();
    m_expressionBar->clearSelection();
    //20200414 bug20294鼠标点击取消focus
    m_expressionBar->getInputEdit()->setFocus();
    switch (key) {
//    case BasicKeypad::Key_0:
//        m_expressionBar->enterNumberEvent("0");
//        break;
//    case BasicKeypad::Key_1:
//        m_expressionBar->enterNumberEvent("1");
//        break;
//    case BasicKeypad::Key_2:
//        m_expressionBar->enterNumberEvent("2");
//        break;
//    case BasicKeypad::Key_3:
//        m_expressionBar->enterNumberEvent("3");
//        break;
//    case BasicKeypad::Key_4:
//        m_expressionBar->enterNumberEvent("4");
//        break;
//    case BasicKeypad::Key_5:
//        m_expressionBar->enterNumberEvent("5");
//        break;
//    case BasicKeypad::Key_6:
//        m_expressionBar->enterNumberEvent("6");
//        break;
//    case BasicKeypad::Key_7:
//        m_expressionBar->enterNumberEvent("7");
//        break;
//    case BasicKeypad::Key_8:
//        m_expressionBar->enterNumberEvent("8");
//        break;
//    case BasicKeypad::Key_9:
//        m_expressionBar->enterNumberEvent("9");
//        break;
//    case BasicKeypad::Key_Plus:
//        m_expressionBar->enterSymbolEvent("+");
//        break;
//    case BasicKeypad::Key_Min:
//        m_expressionBar->enterSymbolEvent("-");
//        break;
//    case BasicKeypad::Key_Mult:
//        m_expressionBar->enterSymbolEvent("*");
//        break;
//    case BasicKeypad::Key_Div:
//        m_expressionBar->enterSymbolEvent("/");
//        break;
//    case BasicKeypad::Key_Percent:
//        m_expressionBar->enterPercentEvent();
//        break;
//    case BasicKeypad::Key_Equals:
//        m_expressionBar->settingLinkage();
//        break;
//    case BasicKeypad::Key_Clear:
//        m_expressionBar->enterClearEvent();
//        break;
//    case BasicKeypad::Key_Backspace:
//        m_expressionBar->enterBackspaceEvent();
//        break;
//    case BasicKeypad::Key_Point:
//        m_expressionBar->enterPointEvent();
//        break;
//    case BasicKeypad::Key_Brackets:
//        m_expressionBar->enterBracketsEvent();
//        break;
    case MemoryKeypad::Key_MS:
//        m_memorylistwidget->generateData(m_expressionBar->getInputEdit()->getMemoryAnswer());
        break;
    case MemoryKeypad::Key_MC:
//        m_memorylistwidget->memoryclean();
        break;
    case MemoryKeypad::Key_Mlist:
//        showListWidget();
//        if (m_keypadLayout->currentIndex() == 1) {
//            m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents);
//            m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents);
//        } else {
//            m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//            m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//        }
//        m_memorylistwidget->setFocus();
        break;
    case MemoryKeypad::Key_Mplus:
//        m_memorylistwidget->memoryplus(m_expressionBar->getInputEdit()->getMemoryAnswer());
        break;
    case MemoryKeypad::Key_Mminus:
//        m_memorylistwidget->memoryminus(m_expressionBar->getInputEdit()->getMemoryAnswer());
        break;
    case MemoryKeypad::Key_MR:
//        m_expressionBar->getInputEdit()->setText(m_memorylistwidget->getfirstnumber());
        break;
    case SciBasicKeyPad::Key_exp:
        m_expressionBar->enterExpEvent(m_deg);
    }
    m_expressionBar->addUndo();
}

void scientificModule::equalButtonPress()
{
    m_expressionBar->settingLinkage();
    m_expressionBar->addUndo();
}

void scientificModule::handleClearStateChanged(bool isAllClear)
{
//    TextButton *btn = static_cast<TextButton *>(m_basicKeypad->button(BasicKeypad::Key_Clear));

//    if (isAllClear) {
//        btn->setText("AC");
//    } else {
//        btn->setText("C");
//    }
}

void scientificModule::handleDegChanged()
{
    TextButton *btn = static_cast<TextButton *>(m_scikeypadwidget->button(SciBasicKeyPad::Key_exp));

    if (btn->text() == "exp") {
        btn->setText("deg");
        m_deg = 1;
    } else if (btn->text() == "deg") {
        btn->setText("rad");
        m_deg = 2;
    } else if (btn->text() == "rad") {
        btn->setText("grad");
        m_deg = 3;
    } else if (btn->text() == "grad") {
        btn->setText("deg");
        m_deg = 1;
    }
}

/*void scientificModule::paintEvent(QPaintEvent *)
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
}*/

void scientificModule::setKeyPress(QKeyEvent *e)
{
    handleEditKeyPress(e);
}

void scientificModule::mAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
    btn->setEnabled(true);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
    btn1->setEnabled(true);
    MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
    btn2->setEnabled(true);
}

void scientificModule::mUnAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MC));
    btn->setEnabled(false);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_MR));
    btn1->setEnabled(false);
    MemoryButton *btn2 = static_cast<MemoryButton *>(m_memoryKeypad->button(MemoryKeypad::Key_Mlist));
    btn2->setEnabled(false);
}

void scientificModule::mousePressEvent(QMouseEvent *event)
{
    if (m_isinsidefun == false) {
        m_scikeypadwidget->funhide();
        m_expressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_memoryKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
    m_insidewidget = false;
    m_expressionBar->getInputEdit()->isExpressionCorrect();
    QWidget::mousePressEvent(event);
}

