#include "memoryitemwidget.h"
#include <QPainter>
#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QEvent>
#include <QContextMenuEvent>
#include <QClipboard>
#include <DGuiApplicationHelper>

MemoryItemWidget::MemoryItemWidget(QWidget *parent)
    : QWidget(parent)
    , btnplus(new MemoryButton(QString("M+"), true, this))
    , btnminus(new MemoryButton(QString("M-"), true, this))
    , btnclean(new MemoryButton(QString("MC"), true, this))
    , label(new QLabel(this))
{
    QVBoxLayout *layV = new QVBoxLayout();
    QHBoxLayout *layH = new QHBoxLayout(this);
    QHBoxLayout *lay = new QHBoxLayout();

    lay->addStretch();
    lay->setSpacing(0);
    lay->addWidget(btnclean);
    lay->addWidget(btnplus);
    lay->addWidget(btnminus);
//    lay->addSpacing(20);
    layV->setMargin(0);
    layV->addWidget(label);
    layV->addStretch();
//    lineedit->setAlignment(Qt::AlignTop);
    QFont font;
    font.setPixelSize(30);
    label->setFont(font);
    label->setAttribute(Qt::WA_TranslucentBackground);

    label->setAlignment(Qt::AlignRight | Qt::AlignTop);
//    lineedit->setAlignment(Qt::AlignCenter | Qt::AlignBottom | Qt::AlignHCenter);
//    lineedit->setTextMargins(0, 0, 10, 6);

    label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
//    layV->addLayout(laylabel);
    layV->addLayout(lay);
//    layV->setContentsMargins(0, 0, 0, 0);
    layH->addLayout(layV);
    layH->addSpacing(20);
    layH->setContentsMargins(0, 0, 0, 6);
    this->setLayout(layH);
//    this->setContentsMargins(0, 0, 0, 0);

    connect(btnplus, &QPushButton::clicked, this, &MemoryItemWidget::plusbtnclicked);
    connect(btnminus, &QPushButton::clicked, this, &MemoryItemWidget::minusbtnclicked);
    connect(btnclean, &QPushButton::clicked, this, &MemoryItemWidget::cleanbtnclicked);
    btnplus->setHidden(true);
    btnminus->setHidden(true);
    btnclean->setHidden(true);
//    m_font.setPixelSize(18);
}

void MemoryItemWidget::enterEvent(QEvent *event)
{
    ishover = true;
    btnplus->setHidden(false);
    btnminus->setHidden(false);
    btnclean->setHidden(false);
    update();
//    this->setAttribute(Qt::WA_StyledBackground, true);
//    this->setStyleSheet("background-color: rgb(245,245,245)");
    QWidget::enterEvent(event);
}

void MemoryItemWidget::leaveEvent(QEvent *event)
{
//    clearFocus();
    ishover = false;
    btnplus->setHidden(true);
    btnminus->setHidden(true);
    btnclean->setHidden(true);
    emit itemchanged(m_themetype);
//    this->setAttribute(Qt::WA_StyledBackground, true);
//    this->setStyleSheet("background-color: rgb(255,255,255)");
    QWidget::leaveEvent(event);
}

void MemoryItemWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
    setFocus();
    ispress = true;
    DPalette pl1 = this->palette();
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, Qt::black);
        pl1.setColor(DPalette::HighlightedText, Qt::black);
    } else {
        pl1.setColor(DPalette::Text, QColor("#FFFFFF"));
        pl1.setColor(DPalette::HighlightedText, QColor("#FFFFFF"));
    }
    pl1.setColor(DPalette::Button, Qt::transparent);
    pl1.setColor(DPalette::Highlight, Qt::transparent);

    // pl.setColor(DPalette::Text,QColor(48,48,48));

    label->setPalette(pl1);
    update();
    QWidget::mousePressEvent(event);
}

void MemoryItemWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
    clearFocus();
    ispress = false;
    DPalette pl1 = this->palette();
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, Qt::black);
        pl1.setColor(DPalette::HighlightedText, Qt::black);
    } else {
        pl1.setColor(DPalette::Text, QColor("#B4B4B4"));
        pl1.setColor(DPalette::HighlightedText, QColor("#B4B4B4"));
    }
    pl1.setColor(DPalette::Button, Qt::transparent);
    pl1.setColor(DPalette::Highlight, Qt::transparent);

    // pl.setColor(DPalette::Text,QColor(48,48,48));

    label->setPalette(pl1);
    QWidget::mouseReleaseEvent(event);
}

void MemoryItemWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QAction *copy = new QAction(tr("Copy"), menu);
    QAction *clean = new QAction(tr("Clear memory item"), menu);
    QAction *plus = new QAction(tr("Add to memory item"), menu);
    QAction *minus = new QAction(tr("Subtract from memory item"), menu);
    menu->addAction(copy);
    menu->addAction(clean);
    menu->addAction(plus);
    menu->addAction(minus);
    if (isExpressionEmpty) {
        plus->setEnabled(false);
        minus->setEnabled(false);
    } else {
        plus->setEnabled(true);
        minus->setEnabled(true);
    }
    connect(copy, &QAction::triggered, this, [ = ]() {
        emit menucopy();
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

void MemoryItemWidget::setTextLabel(QString s)
{
    label->setText(s);
}

QString MemoryItemWidget::textLabel()
{
    return label->text();
}

void MemoryItemWidget::paintEvent(QPaintEvent *e)
{
    QRectF rect = this->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    QColor press, hover;
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        press = QColor(0, 0, 0, 0.2 * 255);
        hover = QColor(0, 0, 0, 0.05 * 255);
    } else {
        press = QColor(255, 255, 255, 0.2 * 255);
        hover = QColor(255, 255, 255, 0.05 * 255);
    }
    if (ishover) {
        painter.setPen(Qt::NoPen);
//        painter.setFont(m_font);
        painter.setBrush(hover);
        painter.drawRect(rect);
        if (ispress) {
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(Qt::NoPen);
            painter.setBrush(press);
            painter.drawRect(rect);

        }
    }
}

void MemoryItemWidget::setLineHight(int line)
{
    label->setFixedHeight(45 * line);
}

void MemoryItemWidget::themetypechanged(int type)
{
    m_themetype = type;
    DPalette pl1 = this->palette();
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, Qt::black);
        pl1.setColor(DPalette::HighlightedText, Qt::black);
    } else {
        pl1.setColor(DPalette::Text, QColor("#B4B4B4"));
        pl1.setColor(DPalette::HighlightedText, QColor("#B4B4B4"));
    }
    pl1.setColor(DPalette::Button, Qt::transparent);
    pl1.setColor(DPalette::Highlight, Qt::transparent);

    // pl.setColor(DPalette::Text,QColor(48,48,48));

    label->setPalette(pl1);
}

void MemoryItemWidget::isexpressionempty(bool b)
{
    isExpressionEmpty = b;
    if (isExpressionEmpty) {
        btnplus->setEnabled(false);
        btnminus->setEnabled(false);
    } else {
        btnplus->setEnabled(true);
        btnminus->setEnabled(true);
    }
}

MemoryItemWidget::~MemoryItemWidget()
{

}
