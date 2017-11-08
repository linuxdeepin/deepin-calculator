#include "mainwindow.h"
#include "separator.h"
#include <dtitlebar.h>

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)                                         
{
    mainWidget = new QWidget;
    mainLayout = new QGridLayout(mainWidget);
    titleBar = new TitleBar;
    expList = new ExpressionList;
    clearButton = new TextButton("C");
    backButton = new TextButton("");
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
    mainLayout->addWidget(new Separator, 1, 0, 1, 4);
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

    backButton->setIcon(QIcon(":/images/delete_normal.svg"));
    backButton->setIconSize(QSize(30, 23));

    titlebar()->setFixedHeight(30);
    titlebar()->setCustomWidget(titleBar, Qt::AlignVCenter, false);

    setWindowIcon(QIcon(":/images/icon.svg"));
    setWindowTitle(tr("Deepin Calculator"));
    setFixedSize(323, 500);
    setCentralWidget(mainWidget);

    connect(zeroButton, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("0");
    });
    connect(num1Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("1");
    });
    connect(num2Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("2");
    });
    connect(num3Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("3");
    });
    connect(num4Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("4");
    });
    connect(num5Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("5");
    });
    connect(num6Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("6");
    });
    connect(num7Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("7");
    });
    connect(num8Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("8");
    });
    connect(num9Button, &QPushButton::clicked, this, [=] {
        onNumberButtonClicked("9");
    });
    connect(plusButton, &QPushButton::clicked, this, [=] {
        onSymbolButtonClicked("+");
    });
    connect(minButton, &QPushButton::clicked, this, [=] {
        onSymbolButtonClicked("-"); 
    });
    connect(multButton, &QPushButton::clicked, this, [=] {
        onSymbolButtonClicked("×");
    });
    connect(divButton, &QPushButton::clicked, this, [=] {
        onSymbolButtonClicked("÷"); 
    });
    connect(bracketsButton, &QPushButton::clicked, this, &MainWindow::onBracketButtonClicked);
    connect(equalButton, &QPushButton::clicked, this, &MainWindow::onEqualButtonClicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);
    connect(pointButton, &QPushButton::clicked, this, &MainWindow::onPointButtonClicked);
    connect(modButton, &QPushButton::clicked, this, &MainWindow::onModButtonClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#FBFBFB"));
    painter.drawRect(QRect(0, 0, rect().width(), 30));
    painter.setBrush(QColor(0, 0, 0, 0.05 * 255));
    painter.drawRect(QRect(rect().x(), 30, rect().width(), height() - 30));
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_0) {
        zeroButton->animateClick();
    } else if (e->key() == Qt::Key_1) {
        num1Button->animateClick();
    } else if (e->key() == Qt::Key_2) {
        num2Button->animateClick();
    } else if (e->key() == Qt::Key_3) {
        num3Button->animateClick();
    } else if (e->key() == Qt::Key_4) {
        num4Button->animateClick();
    } else if (e->key() == Qt::Key_5) {
        num5Button->animateClick();
    } else if (e->key() == Qt::Key_6) {
        num6Button->animateClick();
    } else if (e->key() == Qt::Key_7) {
        num7Button->animateClick();
    } else if (e->key() == Qt::Key_8) {
        num8Button->animateClick();
    } else if (e->key() == Qt::Key_9) {
        num9Button->animateClick();
    } else if (e->key() == Qt::Key_Plus) {
        plusButton->animateClick();
    } else if (e->key() == Qt::Key_Minus || e->key() == Qt::Key_Underscore) {
        minButton->animateClick();
    } else if (e->key() == Qt::Key_Asterisk || e->key() == Qt::Key_X) {
        multButton->animateClick();
    } else if (e->key() == Qt::Key_Slash) {
        divButton->animateClick();
    } else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return || e->key() == Qt::Key_Equal) {
        equalButton->animateClick();
    } else if (e->key() == Qt::Key_Backspace) {
        backButton->animateClick();
    } else if (e->key() == Qt::Key_Period) {
        pointButton->animateClick();
    } else if (e->key() == Qt::Key_Escape) {
        clearButton->animateClick();
    } else if (e->key() == Qt::Key_ParenLeft || e->key() == Qt::Key_ParenRight) {
        bracketsButton->animateClick();
    } else if (e->key() == Qt::Key_Percent) {
        modButton->animateClick();
    } else if (e->key() == Qt::Key_C) {
        if (e->modifiers() == Qt::ControlModifier) {
            expList->copyResultToClipboard();
        }
    }
}

void MainWindow::onNumberButtonClicked(const QString &str)
{
    expList->enterNumberEvent(str);
}

void MainWindow::onBackButtonClicked()
{
    expList->enterBackspaceEvent();
}

void MainWindow::onPointButtonClicked()
{
    expList->enterPointEvent();
}

void MainWindow::onSymbolButtonClicked(const QString &str)
{
    expList->enterSymbolEvent(str);
}

void MainWindow::onClearButtonClicked()
{
    expList->enterClearEvent();
}

void MainWindow::onEqualButtonClicked()
{
    expList->enterEqualEvent();
}

void MainWindow::onBracketButtonClicked()
{
    expList->enterBracketsEvent();
}

void MainWindow::onModButtonClicked()
{
    expList->enterSymbolEvent("%");
}
