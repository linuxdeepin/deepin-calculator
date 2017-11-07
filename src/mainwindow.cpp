#include "mainwindow.h"
#include <dtitlebar.h>

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)                                         
{
    mainWidget = new QWidget;
    mainLayout = new QVBoxLayout(mainWidget);
    firstLayout = new QHBoxLayout;
    secondLayout = new QHBoxLayout;
    thirdLayout = new QHBoxLayout;
    fourthLayout = new QHBoxLayout;
    fifthLayout = new QHBoxLayout;
    titleBar = new TitleBar;
    expList = new ExpressionList;
    separatorWidget = new Separator;
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

    firstLayout->addWidget(clearButton);
    firstLayout->addWidget(modButton);
    firstLayout->addWidget(backButton);
    firstLayout->addWidget(divButton);

    secondLayout->addWidget(num7Button);
    secondLayout->addWidget(num8Button);
    secondLayout->addWidget(num9Button);
    secondLayout->addWidget(multButton);

    thirdLayout->addWidget(num4Button);
    thirdLayout->addWidget(num5Button);
    thirdLayout->addWidget(num6Button);
    thirdLayout->addWidget(minButton);

    fourthLayout->addWidget(num1Button);
    fourthLayout->addWidget(num2Button);
    fourthLayout->addWidget(num3Button);
    fourthLayout->addWidget(plusButton);

    fifthLayout->addWidget(zeroButton);
    fifthLayout->addWidget(pointButton);
    fifthLayout->addWidget(bracketsButton);
    fifthLayout->addWidget(equalButton);

    mainLayout->addWidget(expList);
    mainLayout->addLayout(firstLayout);
    mainLayout->addLayout(secondLayout);
    mainLayout->addLayout(thirdLayout);
    mainLayout->addLayout(fourthLayout);
    mainLayout->addLayout(fifthLayout);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    divButton->setObjectName("SymbolButton");
    multButton->setObjectName("SymbolButton");
    minButton->setObjectName("SymbolButton");
    plusButton->setObjectName("SymbolButton");
    equalButton->setObjectName("EqualButton");
    separatorWidget->setFixedHeight(5);
    backButton->setIcon(QIcon(":/images/delete_normal.svg"));
    backButton->setIconSize(QSize(30, 23));

    titlebar()->setFixedHeight(30);
    titlebar()->setCustomWidget(titleBar, Qt::AlignVCenter, false);

    setWindowIcon(QIcon(":/images/icon.svg"));
    setWindowTitle(tr("Deepin Calculator"));
    setFixedSize(320, 490);
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
    painter.drawRect(rect());
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
