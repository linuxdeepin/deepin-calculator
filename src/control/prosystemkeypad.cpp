#include "prosystemkeypad.h"

const QSize SYSTEMKEYPADSIZE = QSize(451, 279);
const int LEFT_MARGIN = 10; //键盘左边距
const int BOTTOM_MARGIN = 14; //键盘下边距

ProSystemKeypad::ProSystemKeypad(QWidget *parent)
    : DWidget(parent),
      m_layout(new QGridLayout(this))
{
    this->setFixedSize(SYSTEMKEYPADSIZE);
    m_layout->setContentsMargins(LEFT_MARGIN, 0, 0, BOTTOM_MARGIN);
    initUI();
}

ProSystemKeypad::~ProSystemKeypad()
{

}

/**
 * @brief 设置当前显示位数
 * @param system 64, 32, 16, 8
 */
void ProSystemKeypad::setSystem(int system)
{
    QHashIterator<int, ProBitWidget *> i(m_keys);
    if (system == 64) {
        while (i.hasNext()) {
            i.next();
            i.value()->setButtonEnabled(true);
        }
    } else if (system == 32) {
        while (i.hasNext()) {
            i.next();
            if (i.key() == 60 || i.key() == 56 || i.key() == 52 || i.key() == 48)
                i.value()->setButtonEnabled(false);
            else
                i.value()->setButtonEnabled(true);
        }
    } else if (system == 16) {
        while (i.hasNext()) {
            i.next();
            if (i.key() == 60 || i.key() == 56 || i.key() == 52 || i.key() == 48
                    || i.key() == 44 || i.key() == 40 || i.key() == 36 || i.key() == 32)
                i.value()->setButtonEnabled(false);
            else
                i.value()->setButtonEnabled(true);
        }
    } else {
        while (i.hasNext()) {
            i.next();
            if (i.key() == 12 || i.key() == 8 || i.key() == 4 || i.key() == 0)
                i.value()->setButtonEnabled(true);
            else
                i.value()->setButtonEnabled(false);
        }
    }
}

void ProSystemKeypad::initUI()
{
    for (int i = 0; i < 16; i++) {
        ProBitWidget *probitwidget = new ProBitWidget();
        probitwidget->findChild<DLabel *>()->setText(QString::number(60 - i * 4, 10));
        m_layout->addWidget(probitwidget, i / 4, i % 4, Qt::AlignHCenter | Qt::AlignVCenter);

        //每块ProBitWidget对应一个key
        m_keys.insert(60 - i * 4, probitwidget);
    }
}
