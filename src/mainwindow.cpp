#include "mainwindow.h"
#include "dthememanager.h"
#include "dwindowmanagerhelper.h"
#include "utils.h"

#include <QApplication>
#include <QPainter>
#include <QTimer>
#include <dtitlebar.h>

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)
{
    mainWidget = new QWidget;
    mainLayout = new QGridLayout(mainWidget);
    settings = new DSettings;
    titleBar = new TitleBar;
    expList = new ExpressionList;
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

    mainLayout->addWidget(expList, 0, 0, 1, 4);
    mainLayout->addWidget(clearButton, 2, 0);
    mainLayout->addWidget(modButton, 2, 1);
    mainLayout->addWidget(backButton, 2, 2);
    mainLayout->addWidget(divButton, 2, 3);
    mainLayout->addWidget(num7Button, 3, 0);
    mainLayout->addWidget(num8Button, 3, 1);
    mainLayout->addWidget(num9Button, 3, 2);
    mainLayout->addWidget(multButton, 3, 3);
    mainLayout->addWidget(num4Button, 4, 0);
    mainLayout->addWidget(num5Button, 4, 1);
    mainLayout->addWidget(num6Button, 4, 2);
    mainLayout->addWidget(minButton, 4, 3);
    mainLayout->addWidget(num1Button, 5, 0);
    mainLayout->addWidget(num2Button, 5, 1);
    mainLayout->addWidget(num3Button, 5, 2);
    mainLayout->addWidget(plusButton, 5, 3);
    mainLayout->addWidget(zeroButton, 6, 0);
    mainLayout->addWidget(pointButton, 6, 1);
    mainLayout->addWidget(bracketsButton, 6, 2);
    mainLayout->addWidget(equalButton, 6, 3);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(1);

    divButton->setObjectName("SymbolButton");
    multButton->setObjectName("SymbolButton");
    minButton->setObjectName("SymbolButton");
    plusButton->setObjectName("SymbolButton");
    equalButton->setObjectName("EqualButton");

    if (titlebar()) {
        menu = new QMenu;
        themeAction = new QAction(tr("Dark Theme"), this);
        themeAction->setCheckable(true);

        menu->addAction(themeAction);
        menu->addSeparator();

        titlebar()->setMenu(menu);
        titlebar()->setFixedHeight(30);
        titlebar()->setCustomWidget(titleBar, Qt::AlignVCenter, false);
        initTheme();
        initThemeAction();

        connect(themeAction, &QAction::triggered, this, &MainWindow::switchTheme);
    }

    setWindowIcon(QIcon(":/images/deepin-calculator.svg"));
    setWindowTitle(tr("Deepin Calculator"));
    setFixedSize(322, 495);
    setCentralWidget(mainWidget);

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
    connect(bracketsButton, &QPushButton::clicked, expList, &ExpressionList::enterBracketsEvent);
    connect(equalButton, &QPushButton::clicked, expList, &ExpressionList::enterEqualEvent);
    connect(clearButton, &QPushButton::clicked, expList, &ExpressionList::enterClearEvent);
    connect(backButton, &QPushButton::clicked, expList, &ExpressionList::enterBackspaceEvent);
    connect(pointButton, &QPushButton::clicked, expList, &ExpressionList::enterPointEvent);
    connect(expList, &ExpressionList::clearStateChanged, this, &MainWindow::clearButtonStateChanged);
    connect(expList, &ExpressionList::inputKeyPressEvent, this, &MainWindow::keyPressEvent);
}

MainWindow::~MainWindow()
{
    // We don't need clean pointers because applicatibon has exit here.
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);

    // Draw titlebar background color.
    painter.setBrush(QColor(titlebarColor));
    painter.drawRect(QRect(0, 0, rect().width(), 30));

    // Draw separator line.
    painter.setBrush(QColor(separatorColor));
    painter.drawRect(QRect(0, titlebar()->height() + expList->height(), width(), 1));

    const int buttonsTotalHeight = height() - titlebar()->height() - expList->height();
    painter.setBrush(backgroundColor);
    painter.drawRect(QRect(0, height() - buttonsTotalHeight + 1, width(), buttonsTotalHeight));
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_0:
        expList->enterNumberEvent(nullptr, true);
        zeroButton->setDown(true);
        QTimer::singleShot(100, [=] { zeroButton->setDown(false); });
        break;
    case Qt::Key_1:
        expList->enterNumberEvent(nullptr, true);
        num1Button->setDown(true);
        QTimer::singleShot(100, [=] { num1Button->setDown(false); });
        break;
    case Qt::Key_2:
        expList->enterNumberEvent(nullptr, true);
        num2Button->setDown(true);
        QTimer::singleShot(100, [=] { num2Button->setDown(false); });
        break;
    case Qt::Key_3:
        expList->enterNumberEvent(nullptr, true);
        num3Button->setDown(true);
        QTimer::singleShot(100, [=] { num3Button->setDown(false); });
        break;
    case Qt::Key_4:
        expList->enterNumberEvent(nullptr, true);
        num4Button->setDown(true);
        QTimer::singleShot(100, [=] { num4Button->setDown(false); });
        break;
    case Qt::Key_5:
        expList->enterNumberEvent(nullptr, true);
        num5Button->setDown(true);
        QTimer::singleShot(100, [=] { num5Button->setDown(false); });
        break;
    case Qt::Key_6:
        expList->enterNumberEvent(nullptr, true);
        num6Button->setDown(true);
        QTimer::singleShot(100, [=] { num6Button->setDown(false); });
        break;
    case Qt::Key_7:
        expList->enterNumberEvent(nullptr, true);
        num7Button->setDown(true);
        QTimer::singleShot(100, [=] { num7Button->setDown(false); });
        break;
    case Qt::Key_8:
        expList->enterNumberEvent(nullptr, true);
        num8Button->setDown(true);
        QTimer::singleShot(100, [=] { num8Button->setDown(false); });
        break;
    case Qt::Key_9:
        expList->enterNumberEvent(nullptr, true);
        num9Button->setDown(true);
        QTimer::singleShot(100, [=] { num9Button->setDown(false); });
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
        if (!expList->getInputEditText().isEmpty()) {
            backButton->setDown(true);
            QTimer::singleShot(100, [=] { backButton->setDown(false); });
        }
        break;
    case Qt::Key_Period:
        pointButton->animateClick();
        break;
    case Qt::Key_Escape:
        clearButton->animateClick();
        break;
    case Qt::Key_ParenLeft: case Qt::Key_ParenRight:
        expList->setContinue(true);
        bracketsButton->setDown(true);
        QTimer::singleShot(100, this, [=] { bracketsButton->setDown(false); });
        break;
    case Qt::Key_Percent:
        modButton->animateClick();
        break;
    case Qt::Key_C:
        if (e->modifiers() == Qt::ControlModifier) {
            expList->copyResultToClipboard();
        }
        break;
    }
}

void MainWindow::initTheme()
{
    QString theme = settings->getOption("theme");
    DThemeManager::instance()->setTheme(theme);
    if (theme == "light") {
        this->setStyleSheet(Utils::getQssContent(":/qss/light.qss"));
        titlebarColor = "#FBFBFB";
        separatorColor = "#E1E1E1";
        backgroundColor = QColor(0, 0, 0, 0.05 * 255);
    } else {
        this->setStyleSheet(Utils::getQssContent(":/qss/dark.qss"));
        titlebarColor = "#111111";
        separatorColor = "#303030";
        backgroundColor = QColor("#2D2D2D");
    }
}

void MainWindow::initThemeAction()
{
    themeAction->setChecked(settings->getOption("theme") == "dark");
}

void MainWindow::switchTheme()
{
    const QString theme = settings->getOption("theme");

    if (settings->getOption("theme") == "dark") {
        settings->setOption("theme", "light");
        initTheme();
    } else {
        settings->setOption("theme", "dark");
        initTheme();
    }
}

void MainWindow::onNumberButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());

    if (!btn) {
        return;
    }

    expList->enterNumberEvent(btn->text());
}

void MainWindow::onSymbolButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());

    if (!btn) {
        return;
    }

    expList->enterSymbolEvent(btn->text());
}

void MainWindow::clearButtonStateChanged(bool isAllClear)
{
    if (isAllClear) {
        if (expList->getItemsCount() > 0) {
            clearButton->setText("AC");
        }
    } else {
        clearButton->setText("C");
    }
}
