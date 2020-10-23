#include "probitwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

const QSize SIZE = QSize(76, 46);

ProBitWidget::ProBitWidget(QWidget *parent)
    : DWidget(parent),
      m_firstbtn(new BitButton()),
      m_secondbtn(new BitButton()),
      m_thirdbtn(new BitButton()),
      m_fourthbtn(new BitButton()),
      m_label(new DLabel(this))
{
    setFixedSize(SIZE);
    QHBoxLayout *hlay = new QHBoxLayout();
    QVBoxLayout *vlay = new QVBoxLayout(this);
    hlay->addWidget(m_firstbtn);
    hlay->addSpacing(1);
    hlay->addWidget(m_secondbtn);
    hlay->addSpacing(1);
    hlay->addWidget(m_thirdbtn);
    hlay->addSpacing(1);
    hlay->addWidget(m_fourthbtn);
    hlay->addSpacing(1);
    hlay->setMargin(0);
    hlay->setSpacing(4);
    hlay->setContentsMargins(0, 0, 0, 0);
    m_label->setFixedSize(70, 20);
    m_label->setAlignment(Qt::AlignRight);

    QFont font;
    font.setPixelSize(14);
    font.setFamily("Noto Sans");
    m_label->setFont(font);
    vlay->addLayout(hlay);
    vlay->addWidget(m_label);
    vlay->setMargin(0);
    vlay->setSpacing(0);
    vlay->setContentsMargins(0, 0, 0, 0);
}

ProBitWidget::~ProBitWidget()
{

}

/**
 * @brief 设置当前widget，button可用状态
 * @param enabled true-可用　false-不可用
 */
void ProBitWidget::setButtonEnabled(bool enabled)
{
    m_firstbtn->setEnabled(enabled);
    m_secondbtn->setEnabled(enabled);
    m_thirdbtn->setEnabled(enabled);
    m_fourthbtn->setEnabled(enabled);
}
