#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);
    m_iconWidget = new QSvgWidget(":/images/deepin-calculator.svg");

    m_iconWidget->setFixedSize(22, 22);

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(m_iconWidget);
    m_mainLayout->addStretch();
}

TitleBar::~TitleBar()
{
}
