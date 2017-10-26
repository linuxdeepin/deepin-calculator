#include "mainwindow.h"
#include <dtitlebar.h>

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)                                         
{
    mainWidget = new QWidget;
    mainLayout = new QGridLayout(mainWidget);
    titleBar = new TitleBar;
    displayArea = new DisplayArea;

    mainLayout->addWidget(displayArea);

    titlebar()->setCustomWidget(titleBar, Qt::AlignVCenter, false);
    titlebar()->setWindowFlags(titlebar()->windowFlags() & ~Qt::WindowMaximizeButtonHint);

    mainLayout->setContentsMargins(0, 0, 0, 0);

    setFixedSize(325, 475);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
}
