#include "memoryitemwidget.h"

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
    lay->addSpacing(40);
    layV->addLayout(lay);
    this->setLayout(layV);
    connect(btnplus, &QPushButton::clicked, this, &MemoryItemWidget::plusbtnclicked);
    connect(btnminus, &QPushButton::clicked, this, &MemoryItemWidget::minusbtnclicked);
    connect(btnclean, &QPushButton::clicked, this, &MemoryItemWidget::cleanbtnclicked);
    btnplus->setHidden(true);
    btnminus->setHidden(true);
    btnclean->setHidden(true);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::gray);
    this->setPalette(pal);
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

//void ItemWidget::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    painter.setBrush(QBrush(QColor(125,125,125,254)));
//}

MemoryItemWidget::~MemoryItemWidget()
{

}
