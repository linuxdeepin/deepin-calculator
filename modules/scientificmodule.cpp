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
//    m_scikeypadwidget = new ScientificKeyPad;
    m_scikeypadwidget = new ScientificKeyPad;
    m_insidewidget = false;
    m_scihiswidget = new SciHistoryWidget(this);
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

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged, this,
            &scientificModule::initTheme);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_scikeypadwidget, &ScientificKeyPad::buttonThemeChanged);
    connect(m_sciexpressionBar, &SciExpressionBar::keyPress, this, &scientificModule::handleEditKeyPress);
    connect(m_sciexpressionBar, &SciExpressionBar::clearStateChanged, this,
            &scientificModule::handleClearStateChanged);
    connect(m_sciexpressionBar, &SciExpressionBar::turnDeg, this, &scientificModule::handleDegChanged);
//    connect(m_basicKeypad, &BasicKeypad::buttonPressed, this,
//            &scientificModule::handleKeypadButtonPress);
//    connect(m_basicKeypad, &BasicKeypad::equalPressed, this, &scientificModule::equalButtonPress);
//    connect(m_basicKeypad, &BasicKeypad::moveLeft, [ = ] { m_sciexpressionBar->moveLeft(); });
//    connect(m_basicKeypad, &BasicKeypad::moveRight, [ = ] { m_sciexpressionBar->moveRight(); });
    connect(m_scikeypadwidget, &ScientificKeyPad::buttonPressed, this,
            &scientificModule::handleKeypadButtonPress);
//    connect(m_memorylistwidget, &MemoryWidget::widgetplus, this, [ = ](int row) {
//        m_memorylistwidget->widgetplusslot(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer());
//    });
//    connect(m_memorylistwidget, &MemoryWidget::widgetminus, this, [ = ](int row) {
//        m_memorylistwidget->widgetminusslot(row, m_sciexpressionBar->getInputEdit()->getMemoryAnswer());
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
//        m_sciexpressionBar->getInputEdit()->setText(str);
//        m_sciexpressionBar->getInputEdit()->setFocus();
//        if (m_keypadLayout->currentIndex() == 1) {
//            m_keypadLayout->setCurrentIndex(0);
//            m_scikeypadwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//            m_sciexpressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//            MemoryButton *btn = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MC));
//            btn->setEnabled(true);
//            MemoryButton *btn1 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MR));
//            btn1->setEnabled(true);
//            MemoryButton *btn2 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mplus));
//            btn2->setEnabled(true);
//            MemoryButton *btn3 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mminus));
//            btn3->setEnabled(true);
//            MemoryButton *btn4 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MS));
//            btn4->setEnabled(true);
//        }
//    });
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
        } else {
            MemoryButton *btn2 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mplus));
            btn2->setEnabled(false);
            MemoryButton *btn3 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_Mmin));
            btn3->setEnabled(false);
            MemoryButton *btn4 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MS));
            btn4->setEnabled(false);
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
//    case BasicKeypad::Key_0:
//        m_sciexpressionBar->enterNumberEvent("0");
//        break;
//    case BasicKeypad::Key_1:
//        m_sciexpressionBar->enterNumberEvent("1");
//        break;
//    case BasicKeypad::Key_2:
//        m_sciexpressionBar->enterNumberEvent("2");
//        break;
//    case BasicKeypad::Key_3:
//        m_sciexpressionBar->enterNumberEvent("3");
//        break;
//    case BasicKeypad::Key_4:
//        m_sciexpressionBar->enterNumberEvent("4");
//        break;
//    case BasicKeypad::Key_5:
//        m_sciexpressionBar->enterNumberEvent("5");
//        break;
//    case BasicKeypad::Key_6:
//        m_sciexpressionBar->enterNumberEvent("6");
//        break;
//    case BasicKeypad::Key_7:
//        m_sciexpressionBar->enterNumberEvent("7");
//        break;
//    case BasicKeypad::Key_8:
//        m_sciexpressionBar->enterNumberEvent("8");
//        break;
//    case BasicKeypad::Key_9:
//        m_sciexpressionBar->enterNumberEvent("9");
//        break;
//    case BasicKeypad::Key_Plus:
//        m_sciexpressionBar->enterSymbolEvent("+");
//        break;
//    case BasicKeypad::Key_Min:
//        m_sciexpressionBar->enterSymbolEvent("-");
//        break;
//    case BasicKeypad::Key_Mult:
//        m_sciexpressionBar->enterSymbolEvent("*");
//        break;
//    case BasicKeypad::Key_Div:
//        m_sciexpressionBar->enterSymbolEvent("/");
//        break;
//    case BasicKeypad::Key_Percent:
//        m_sciexpressionBar->enterPercentEvent();
//        break;
//    case BasicKeypad::Key_Equals:
//        m_sciexpressionBar->settingLinkage();
//        break;
//    case BasicKeypad::Key_Clear:
//        m_sciexpressionBar->enterClearEvent();
//        break;
//    case BasicKeypad::Key_Backspace:
//        m_sciexpressionBar->enterBackspaceEvent();
//        break;
//    case BasicKeypad::Key_Point:
//        m_sciexpressionBar->enterPointEvent();
//        break;
//    case BasicKeypad::Key_Brackets:
//        m_sciexpressionBar->enterBracketsEvent();
//        break;
    case ScientificKeyPad::Key_MS:
//        m_memorylistwidget->generateData(m_sciexpressionBar->getInputEdit()->getMemoryAnswer());
        break;
    case ScientificKeyPad::Key_MC:
//        m_memorylistwidget->memoryclean();
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
        break;
    case ScientificKeyPad::Key_Mplus:
//        m_memorylistwidget->memoryplus(m_sciexpressionBar->getInputEdit()->getMemoryAnswer());
        break;
    case ScientificKeyPad::Key_Mmin:
//        m_memorylistwidget->memoryminus(m_sciexpressionBar->getInputEdit()->getMemoryAnswer());
        break;
    case ScientificKeyPad::Key_MR:
//        m_sciexpressionBar->getInputEdit()->setText(m_memorylistwidget->getfirstnumber());
        break;
    case ScientificKeyPad::Key_exp:
        m_sciexpressionBar->enterExpEvent(m_deg);
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
//    TextButton *btn = static_cast<TextButton *>(m_basicKeypad->button(BasicKeypad::Key_Clear));

//    if (isAllClear) {
//        btn->setText("AC");
//    } else {
//        btn->setText("C");
//    }
}

void scientificModule::handleDegChanged()
{
    TextButton *btn = static_cast<TextButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_exp));

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
}

void scientificModule::mUnAvailableEvent()
{
    MemoryButton *btn = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MC));
    btn->setEnabled(false);
    MemoryButton *btn1 = static_cast<MemoryButton *>(m_scikeypadwidget->button(ScientificKeyPad::Key_MR));
    btn1->setEnabled(false);
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

