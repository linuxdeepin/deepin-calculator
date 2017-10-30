#include "mainwindow.h"
#include <dtitlebar.h>
#include "listitem.h"

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)                                         
{
    mainWidget = new QWidget;
    mainLayout = new QGridLayout(mainWidget);
    titleBar = new TitleBar;
    displayArea = new DisplayArea;
    clearButton= new TextButton("C");
    backButton= new TextButton("←");
    modButton= new TextButton("%");
    divButton= new TextButton("÷");
    num7Button= new TextButton("7");
    num8Button= new TextButton("8");
    num9Button= new TextButton("9");
    multButton= new TextButton("×");
    num4Button= new TextButton("4");
    num5Button= new TextButton("5");
    num6Button= new TextButton("6");
    minButton= new TextButton("－");
    num1Button= new TextButton("1");
    num2Button= new TextButton("2");
    num3Button= new TextButton("3");
    plusButton= new TextButton("＋");
    zeroButton= new TextButton("0");
    pointButton= new TextButton(".");
    bracketsButton= new TextButton("( )");
    equalButton= new TextButton("＝");

    divButton->setObjectName("SymbolButton");
    multButton->setObjectName("SymbolButton");
    minButton->setObjectName("SymbolButton");
    plusButton->setObjectName("SymbolButton");
    equalButton->setObjectName("EqualButton");

    mainLayout->addWidget(displayArea, 0, 0, 1, 4);
    mainLayout->addWidget(clearButton, 1, 0);
    mainLayout->addWidget(backButton, 1, 1);
    mainLayout->addWidget(modButton, 1, 2);
    mainLayout->addWidget(divButton, 1, 3);
    mainLayout->addWidget(num7Button, 2, 0);
    mainLayout->addWidget(num8Button, 2, 1);
    mainLayout->addWidget(num9Button, 2, 2);
    mainLayout->addWidget(multButton, 2, 3);
    mainLayout->addWidget(num4Button, 3, 0);
    mainLayout->addWidget(num5Button, 3, 1);
    mainLayout->addWidget(num6Button, 3, 2);
    mainLayout->addWidget(minButton, 3, 3);
    mainLayout->addWidget(num1Button, 4, 0);
    mainLayout->addWidget(num2Button, 4, 1);
    mainLayout->addWidget(num3Button, 4, 2);
    mainLayout->addWidget(plusButton, 4, 3);
    mainLayout->addWidget(zeroButton, 5, 0);
    mainLayout->addWidget(pointButton, 5, 1);
    mainLayout->addWidget(bracketsButton, 5, 2);
    mainLayout->addWidget(equalButton, 5, 3);

    titlebar()->setCustomWidget(titleBar, Qt::AlignVCenter, false);
    titlebar()->setWindowFlags(titlebar()->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    titlebar()->setSeparatorVisible(true);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setFixedSize(330, 480);
    setCentralWidget(mainWidget);

    displayArea->scrollToEnd();

    connect(equalButton, &QPushButton::clicked, this, [=]{
                                                          displayArea->addItem(new ListItem);
                                                          displayArea->scrollToEnd();
                                                      });
}

MainWindow::~MainWindow()
{
}
