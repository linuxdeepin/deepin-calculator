#include "mainwindow.h"
#include "dthememanager.h"
#include "dwindowmanagerhelper.h"
#include "utils.h"

#include <QApplication>
#include <QPainter>
#include <dtitlebar.h>
#include <QLabel>
#include <DHiDPIHelper>

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)
{
    m_mainWidget = new QWidget;
    m_mainLayout = new QGridLayout(m_mainWidget);
    m_settings = new DSettings;
    m_expList = new ExpressionList;
    clearButton = new TextButton("C");
    backButton = new BackButton;
    modButton = new TextButton("%");
    divButton = new TextButton("÷");
    num7Button = new TextButton("7");
    num8Button = new TextButton("8");
    num9Button = new TextButton("9");
    multButton = new TextButton("×");
    num4Button = new TextButton("4");
    num5Button = new TextButton("5");
    num6Button = new TextButton("6");
    minButton = new TextButton("－");
    num1Button = new TextButton("1");
    num2Button = new TextButton("2");
    num3Button = new TextButton("3");
    plusButton = new TextButton("＋");
    zeroButton = new TextButton("0");
    pointButton = new TextButton(".");
    bracketsButton = new TextButton("( )");
    equalButton = new TextButton("＝");

    m_mainLayout->addWidget(m_expList, 0, 0, 1, 4);
    m_mainLayout->addWidget(clearButton, 2, 0);
    m_mainLayout->addWidget(modButton, 2, 1);
    m_mainLayout->addWidget(backButton, 2, 2);
    m_mainLayout->addWidget(divButton, 2, 3);
    m_mainLayout->addWidget(num7Button, 3, 0);
    m_mainLayout->addWidget(num8Button, 3, 1);
    m_mainLayout->addWidget(num9Button, 3, 2);
    m_mainLayout->addWidget(multButton, 3, 3);
    m_mainLayout->addWidget(num4Button, 4, 0);
    m_mainLayout->addWidget(num5Button, 4, 1);
    m_mainLayout->addWidget(num6Button, 4, 2);
    m_mainLayout->addWidget(minButton, 4, 3);
    m_mainLayout->addWidget(num1Button, 5, 0);
    m_mainLayout->addWidget(num2Button, 5, 1);
    m_mainLayout->addWidget(num3Button, 5, 2);
    m_mainLayout->addWidget(plusButton, 5, 3);
    m_mainLayout->addWidget(zeroButton, 6, 0);
    m_mainLayout->addWidget(pointButton, 6, 1);
    m_mainLayout->addWidget(bracketsButton, 6, 2);
    m_mainLayout->addWidget(equalButton, 6, 3);
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(1);

    divButton->setObjectName("SymbolButton");
    multButton->setObjectName("SymbolButton");
    minButton->setObjectName("SymbolButton");
    plusButton->setObjectName("SymbolButton");
    equalButton->setObjectName("EqualButton");

    if (titlebar()) {
        m_menu = new QMenu;
        m_themeAction = new QAction(tr("Dark Theme"), this);
        m_themeAction->setCheckable(true);

        m_menu->addAction(m_themeAction);
        m_menu->addSeparator();

        QLabel *icon = new QLabel;
        icon->setPixmap(DHiDPIHelper::loadNxPixmap(":/images/deepin-calculator.svg"));

        titlebar()->setMenu(m_menu);
        titlebar()->setFixedHeight(30);
        titlebar()->setCustomWidget(icon, Qt::AlignVCenter, false);
        initTheme();
        initThemeAction();

        connect(m_themeAction, &QAction::triggered, this, &MainWindow::switchTheme);
    }

    setWindowIcon(QIcon(":/images/deepin-calculator.svg"));
    setWindowTitle(tr("Deepin Calculator"));
    setCentralWidget(m_mainWidget);

    if (m_settings->getOption("Resize").toBool()) {
        resize(322, 495);
    } else {
        setFixedSize(322, 495);
    }

    connect(zeroButton, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num1Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num2Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num3Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num4Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num5Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num6Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num7Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num8Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(num9Button, &QPushButton::clicked, this, &MainWindow::onNumberButtonClicked);
    connect(plusButton, &QPushButton::clicked, this, &MainWindow::onSymbolButtonClicked);
    connect(minButton, &QPushButton::clicked, this, &MainWindow::onSymbolButtonClicked);
    connect(multButton, &QPushButton::clicked, this, &MainWindow::onSymbolButtonClicked);
    connect(divButton, &QPushButton::clicked, this, &MainWindow::onSymbolButtonClicked);
    connect(modButton, &QPushButton::clicked, this, &MainWindow::onSymbolButtonClicked);
    connect(bracketsButton, &QPushButton::clicked, m_expList, &ExpressionList::enterBracketsEvent);
    connect(equalButton, &QPushButton::clicked, m_expList, &ExpressionList::enterEqualEvent);
    connect(clearButton, &QPushButton::clicked, m_expList, &ExpressionList::enterClearEvent);
    connect(backButton, &QPushButton::clicked, m_expList, &ExpressionList::enterBackspaceEvent);
    connect(pointButton, &QPushButton::clicked, m_expList, &ExpressionList::enterPointEvent);
    connect(m_expList, &ExpressionList::clearStateChanged, this, &MainWindow::clearButtonStateChanged);
    connect(m_expList, &ExpressionList::inputKeyPressEvent, this, &MainWindow::keyPressEvent);
}

MainWindow::~MainWindow()
{
    // We don't need clean pointers because applicatibon has exit here.
}

void MainWindow::paintEvent(QPaintEvent * event)
{
    DMainWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);

    // Draw titlebar background color.
    painter.setBrush(QColor(m_titlebarColor));
    painter.drawRect(rect());

    // Draw separator line.
    painter.setBrush(QColor(m_separatorColor));
    painter.drawRect(QRect(0, titlebar()->height() + m_expList->height(), width(), 1));

    const int buttonsTotalHeight = height() - titlebar()->height() - m_expList->height();
    painter.setBrush(m_backgroundColor);
    painter.drawRect(QRect(0, height() - buttonsTotalHeight + 1, width(), buttonsTotalHeight));
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_0:
        m_expList->enterNumberEvent(nullptr, true);
        zeroButton->animate();
        break;
    case Qt::Key_1:
        m_expList->enterNumberEvent(nullptr, true);
        num1Button->animate();
        break;
    case Qt::Key_2:
        m_expList->enterNumberEvent(nullptr, true);
        num2Button->animate();
        break;
    case Qt::Key_3:
        m_expList->enterNumberEvent(nullptr, true);
        num3Button->animate();
        break;
    case Qt::Key_4:
        m_expList->enterNumberEvent(nullptr, true);
        num4Button->animate();
        break;
    case Qt::Key_5:
        m_expList->enterNumberEvent(nullptr, true);
        num5Button->animate();
        break;
    case Qt::Key_6:
        m_expList->enterNumberEvent(nullptr, true);
        num6Button->animate();
        break;
    case Qt::Key_7:
        m_expList->enterNumberEvent(nullptr, true);
        num7Button->animate();
        break;
    case Qt::Key_8:
        m_expList->enterNumberEvent(nullptr, true);
        num8Button->animate();
        break;
    case Qt::Key_9:
        m_expList->enterNumberEvent(nullptr, true);
        num9Button->animate();
        break;
    case Qt::Key_Plus:
        plusButton->animateClick();
        break;
    case Qt::Key_Minus: case Qt::Key_Underscore:
        minButton->animateClick();
        break;
    case Qt::Key_Asterisk: case Qt::Key_X:
        multButton->animateClick();
        break;
    case Qt::Key_Slash:
        divButton->animateClick();
        break;
    case Qt::Key_Enter: case Qt::Key_Return: case Qt::Key_Equal:
        equalButton->animateClick();
        break;
    case Qt::Key_Backspace:
        if (!m_expList->getInputEditText().isEmpty()) {
            backButton->animate();
        }
        break;
    case Qt::Key_Period:
        pointButton->animateClick();
        break;
    case Qt::Key_Escape:
        clearButton->animateClick();
        break;
    case Qt::Key_ParenLeft: case Qt::Key_ParenRight:
        m_expList->setContinue(true);
        bracketsButton->animate();
        break;
    case Qt::Key_Percent:
        modButton->animateClick();
        break;
    case Qt::Key_C:
        if (e->modifiers() == Qt::ControlModifier) {
            m_expList->copyResultToClipboard();
        }
        break;
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    m_expList->update();
}

void MainWindow::initTheme()
{
    QString theme = m_settings->getOption("theme").toString();
    DThemeManager::instance()->setTheme(theme);

    const bool isBlur = m_settings->getOption("EnableBlur").toBool();

    if (isBlur) {
        setAttribute(Qt::WA_TranslucentBackground);
        setEnableBlurWindow(true);
    }

    if (theme == "light") {
        this->setStyleSheet(Utils::getQssContent(":/qss/light.qss"));
        m_titlebarColor = isBlur ? QColor(251,251,251, 0.6 * 255) : QColor(251,251,251);
        m_separatorColor = "#E1E1E1";
        m_backgroundColor = QColor(0, 0, 0, 0.05 * 255);
    } else {
        this->setStyleSheet(Utils::getQssContent(":/qss/dark.qss"));
        m_titlebarColor = isBlur ? QColor(17, 17, 17, 0.6 * 255) : QColor(17, 17, 17);
        m_separatorColor = "#303030";
        m_backgroundColor = QColor("#2D2D2D");
    }
}

void MainWindow::initThemeAction()
{
    m_themeAction->setChecked(m_settings->getOption("theme") == "dark");
}

void MainWindow::switchTheme()
{
    const QString theme = m_settings->getOption("theme").toString();

    if (m_settings->getOption("theme") == "dark") {
        m_settings->setOption("theme", "light");
        initTheme();
    } else {
        m_settings->setOption("theme", "dark");
        initTheme();
    }
}

void MainWindow::onNumberButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());

    if (!btn) {
        return;
    }

    m_expList->enterNumberEvent(btn->text());
}

void MainWindow::onSymbolButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());

    if (!btn) {
        return;
    }

    m_expList->enterSymbolEvent(btn->text());
}

void MainWindow::clearButtonStateChanged(bool isAllClear)
{
    if (isAllClear) {
        if (m_expList->getItemsCount() > 0) {
            clearButton->setText("AC");
        }
    } else {
        clearButton->setText("C");
    }
}
