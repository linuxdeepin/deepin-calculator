#include "memoryitemwidget.h"
#include <QPainter>

MemoryItemWidget::MemoryItemWidget(QWidget *parent)
    : QWidget(parent)
    , btnplus(new MemoryButton(QString("M+"), true, this))
    , btnminus(new MemoryButton(QString("M-"), true, this))
    , btnclean(new MemoryButton(QString("MC"), true, this))
{
    QVBoxLayout *layV = new QVBoxLayout(this);
    layV->addStretch();
    QHBoxLayout *lay = new QHBoxLayout();
    btnplus->setText("M+");
//    btnplus->setFixedSize(40, 30);
    btnminus->setText("M-");
//    btnminus->setFixedSize(40, 30);
    btnclean->setText("MC");
//    btnclean->setFixedSize(40, 30);
    lay->addStretch();
    lay->setSpacing(0);
    lay->addWidget(btnclean);
    lay->addWidget(btnplus);
    lay->addWidget(btnminus);
    lay->addSpacing(20);
    layV->addLayout(lay);
    this->setLayout(layV);
    connect(btnplus, &QPushButton::clicked, this, &MemoryItemWidget::plusbtnclicked);
    connect(btnminus, &QPushButton::clicked, this, &MemoryItemWidget::minusbtnclicked);
    connect(btnclean, &QPushButton::clicked, this, &MemoryItemWidget::cleanbtnclicked);
    btnplus->setHidden(true);
    btnminus->setHidden(true);
    btnclean->setHidden(true);
}

void MemoryItemWidget::enterEvent(QEvent *event)
{
    btnplus->setHidden(false);
    btnminus->setHidden(false);
    btnclean->setHidden(false);
    QWidget::enterEvent(event);
}

void MemoryItemWidget::leaveEvent(QEvent *event)
{
    btnplus->setHidden(true);
    btnminus->setHidden(true);
    btnclean->setHidden(true);
    QWidget::leaveEvent(event);
}

MemoryItemWidget::~MemoryItemWidget()
{

}
