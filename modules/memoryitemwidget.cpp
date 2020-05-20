#include "memoryitemwidget.h"
#include <QPainter>
#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QEvent>
#include <QContextMenuEvent>
#include <QClipboard>

MemoryItemWidget::MemoryItemWidget(QWidget *parent)
    : QWidget(parent)
    , btnplus(new MemoryButton(QString("M+"), true, this))
    , btnminus(new MemoryButton(QString("M-"), true, this))
    , btnclean(new MemoryButton(QString("MC"), true, this))
{
    QVBoxLayout *layV = new QVBoxLayout(this);
    layV->addStretch();
    QHBoxLayout *lay = new QHBoxLayout();
//    btnplus->setText("M+");
//    btnplus->setFixedSize(40, 30);
//    btnminus->setText("M-");
//    btnminus->setFixedSize(40, 30);
//    btnclean->setText("MC");
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
    emit itemchanged(m_themetype);
    QWidget::leaveEvent(event);
}

void MemoryItemWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QAction *copy = new QAction("复制", menu);
    QAction *clean = new QAction("清除内存项", menu);
    QAction *plus = new QAction("添加到内存项", menu);
    QAction *minus = new QAction("从内存项中减去", menu);
    menu->addAction(copy);
    menu->addAction(clean);
    menu->addAction(plus);
    menu->addAction(minus);
    connect(copy, &QAction::triggered, this, [ = ]() {
        emit menucopy();
//        QClipboard *clipboard = QApplication::clipboard();
//        QString originalText = clipboard->text();
//        clipboard->setText(m_listwidget->itemAt(event->pos())->data(Qt::EditRole).toString().remove("\n"));
    });
    connect(clean, &QAction::triggered, this, [ = ]() {
        emit menuclean();
    });
    connect(plus, &QAction::triggered, this, [ = ]() {
        emit menuplus();
    });
    connect(minus, &QAction::triggered, this, [ = ]() {
        emit menuminus();
    });
    menu->exec(event->globalPos());
    delete menu;
}

void MemoryItemWidget::themetypechanged(int type)
{
    m_themetype = type;
}

MemoryItemWidget::~MemoryItemWidget()
{

}
