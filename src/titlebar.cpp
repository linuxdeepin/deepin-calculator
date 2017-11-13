#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    iconWidget = new QSvgWidget(":/images/deepin-calculator.svg");

    iconWidget->setFixedSize(22, 22);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(iconWidget);
    layout->addStretch();
}

TitleBar::~TitleBar()
{
}
