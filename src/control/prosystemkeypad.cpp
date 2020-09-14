#include "prosystemkeypad.h"

const QSize SYSTEMKEYPADSIZE = QSize(451, 279);
ProSystemKeypad::ProSystemKeypad(QWidget *parent)
    : DWidget(parent),
      m_layout(new QGridLayout(this))
{
    this->setFixedSize(SYSTEMKEYPADSIZE);
//    m_layout->setMargin(0);
//    m_layout->setSpacing(KEYPAD_SPACING);
    m_layout->setContentsMargins(0, 0, 0, 0);
    initUI();
}

ProSystemKeypad::~ProSystemKeypad()
{

}

void ProSystemKeypad::initUI()
{
    for (int i = 0; i < 16; i++) {
        ProBitWidget *probitwidget = new ProBitWidget();
        probitwidget->findChild<DLabel *>()->setText(QString::number(60 - i * 4, 10));
        m_layout->addWidget(probitwidget, i / 4, i % 4, Qt::AlignHCenter | Qt::AlignVCenter);
    }
}
