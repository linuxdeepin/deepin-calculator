#include "memorywidget.h"

#include <QDebug>
#include <QFont>
#include <QModelIndex>
#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QEvent>
#include <QPen>
#include <QContextMenuEvent>
#include <QMenu>
#include <QClipboard>
#include "../utils.h"

MemoryWidget::MemoryWidget(QWidget *parent)
    : QWidget(parent)
    , m_listwidget(new MemoryListWidget(this))
    , m_clearbutton(new DPushButton(this))
    , m_isempty(true)
{
    m_evaluator = Evaluator::instance();
    this->setContentsMargins(0, 0, 0, 0);
//    this->setFixedSize(344, 300);
//    m_pDeleteBtn->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/images/light/+_normal.svg);} \
//                                 QPushButton:hover {image:url(:/images/light/+_hover.svg);} \
//                                 QPushButton:pressed {image:url(:/images/light/+_press.svg);}");
    QVBoxLayout *lay = new QVBoxLayout(this);
    QHBoxLayout *layH = new QHBoxLayout();

    lay->setSpacing(0);
    lay->setMargin(0);
    lay->addWidget(m_listwidget);
//    lay->addLayout(layH);
    m_listwidget->setFrameShape(QFrame::NoFrame);
    m_listwidget->setFixedHeight(260);
    m_listwidget->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listwidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_listwidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listwidget->setAutoScroll(false);
    m_listwidget->setSelectionRectVisible(false);
    m_listwidget->setFocusPolicy(Qt::NoFocus);
    m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:rgb(150,150,150);} \
                                 QListWidget::item:hover{color:black;background-color:rgb(180,180,180);} \
                                 QListWidget{color:black;background-color:rgb(150,150,150);}");
    lay->addStretch();
    layH->addStretch();
    layH->addWidget(m_clearbutton);
    m_clearbutton->setFixedSize(30, 30);
    m_clearbutton->setStyleSheet("QPushButton {border:none;background-color: transparent;image:url(:/images/light/clear_normal.svg);} \
                                  QPushButton:hover {border: 1px solid #000000;image:url(:/images/light/clear_hover.svg);} \
                                  QPushButton:pressed {image:url(:/images/light/clear_press.svg);}");
    connect(m_clearbutton, &DPushButton::clicked, this, [ = ]() {
        m_listwidget->clear();
        m_listwidget->addItem("内存中没有内容");
        m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
        m_isempty = true;
        emit mListUnavailable();
    });
    lay->addLayout(layH);
    this->setLayout(lay);
    connect(m_listwidget, &QListWidget::itemClicked, this, [ = ](QListWidgetItem * item) {
        QString str = item->data(Qt::DisplayRole).toString();
        if (item->flags() != Qt::NoItemFlags)
            emit itemclick(str);
    });
    connect(m_listwidget, &QListWidget::itemPressed, this, [ = ](QListWidgetItem * item) {
        m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:rgb(150,150,150);} \
                                     QListWidget::item:selected{color:black;background-color:rgb(115,115,115);} \
                                     QListWidget{color:black;background-color:rgb(150,150,150);}");
    });
}

void MemoryWidget::generateData(const QString str)
{
    if (m_isempty == true) {
        m_listwidget->clear();
    }
    m_isempty = false;
    emit mListAvailable();
    QListWidgetItem *item1 = new QListWidgetItem();
    if (str == QString()) {
        item1->setData(Qt::DisplayRole, "0");
    } else {
        item1->setData(Qt::DisplayRole, str);
    }
    item1->setTextAlignment(Qt::AlignRight | Qt::AlignTop);
    QFont font;
    font.setPixelSize(24);
    item1->setFont(font);
    item1->setSizeHint(QSize(344, 130));
//    item1->setBackgroundColor(QColor(125,125,125,254));
//    item1->setFlags(Qt::NoItemFlags);
    MemoryItemWidget *widget = new MemoryItemWidget(this);
    widget->setFixedSize(344, 130);
    m_listwidget->insertItem(0, item1);
    m_listwidget->setItemWidget(item1, widget);
    connect(widget, &MemoryItemWidget::plusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryWidget::widgetplus(row);
    });
    connect(widget, &MemoryItemWidget::minusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryWidget::widgetminus(row);
    });
    connect(widget, &MemoryItemWidget::cleanbtnclicked, this, [ = ]() {
        m_listwidget->takeItem(m_listwidget->row(item1));
        if (m_listwidget->count() == 0) {
            m_listwidget->addItem("内存中没有内容");
            m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
            m_isempty = true;
            emit mListUnavailable();
        }
    });
    connect(widget, &MemoryItemWidget::itemchanged, this, [ = ]() {
        m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:rgb(150,150,150);} \
                                     QListWidget::item:hover{color:black;background-color:rgb(180,180,180);} \
                                     QListWidget{color:black;background-color:rgb(150,150,150);}");
    });
}

void MemoryWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(QBrush(QColor(150, 150, 150)), 0);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(150, 150, 150)));
    QRect rect(this->rect().left(), this->rect().top(), this->rect().width(), this->rect().bottom() + 30);
    painter.drawRect(rect);
    QWidget::paintEvent(event);
}

void MemoryWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_type = -1;
        QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
        m_mousepoint = pEvent->pos();

        QRect rect(this->frameGeometry());
        if (rect.contains(m_mousepoint) == true)
            emit insidewidget();
    }
    QWidget::mousePressEvent(event);
}

void MemoryWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_listwidget->itemAt(event->pos()) && !m_isempty) {
        QMenu *menu = new QMenu(this);
        QAction *copy = new QAction("复制");
        QAction *clean = new QAction("清除内存项");
        QAction *plus = new QAction("添加到内存项");
        QAction *minus = new QAction("从内存项中减去");
        menu->addAction(copy);
        menu->addAction(clean);
        menu->addAction(plus);
        menu->addAction(minus);
        connect(copy, &QAction::triggered, this, [ = ]() {
            QClipboard *clipboard = QApplication::clipboard();
            QString originalText = clipboard->text();
            clipboard->setText(m_listwidget->itemAt(event->pos())->data(Qt::EditRole).toString());
        });
        connect(clean, &QAction::triggered, this, [ = ]() {
            m_listwidget->takeItem(m_listwidget->row(m_listwidget->itemAt(event->pos())));
            if (m_listwidget->count() == 0) {
                m_listwidget->addItem("内存中没有内容");
                m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
                m_isempty = true;
                emit mListUnavailable();
            }
        });
        connect(plus, &QAction::triggered, this, [ = ]() {
            int row = m_listwidget->row(m_listwidget->itemAt(event->pos()));
            emit MemoryWidget::widgetplus(row);
        });
        connect(minus, &QAction::triggered, this, [ = ]() {
            int row = m_listwidget->row(m_listwidget->itemAt(event->pos()));
            emit MemoryWidget::widgetminus(row);
        });
        menu->exec(event->globalPos());
        delete menu;
    }
}

void MemoryWidget::memoryplus(const QString str)
{
    QString str1 = (str == QString()) ? "0" : str;
    if (m_isempty == false) {
        QString exp = QString(m_listwidget->item(0)->data(Qt::EditRole).toString() + "+" + str1);
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+")
                       .replace(QString::fromUtf8("－"), "-");
        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
    } else {
        m_listwidget->clear();
        generateData(str1);
    }
}

void MemoryWidget::memoryminus(const QString str)
{
    QString str1 = (str == QString()) ? "0" : str;
    if (m_isempty == false) {
        QString exp = QString(m_listwidget->item(0)->data(Qt::EditRole).toString() + "-" + str1);
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+")
                       .replace(QString::fromUtf8("－"), "-");
        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
    } else {
        m_listwidget->clear();
        generateData(QString());
        QString exp = QString("0-" + str1);
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+")
                       .replace(QString::fromUtf8("－"), "-");
        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
    }
}

void MemoryWidget::memoryclean()
{
    m_listwidget->clear();
    m_isempty = true;
    emit mListUnavailable();
}

QString MemoryWidget::getfirstnumber()
{
    if (m_isempty == false) {
        return m_listwidget->item(0)->data(Qt::EditRole).toString();
    } else {
        return QString();
    }
}

void MemoryWidget::widgetplusslot(int row, const QString str)
{
    if (str == QString())
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    else {
        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "+" + str);
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        qDebug() << formatResult;
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+")
                       .replace(QString::fromUtf8("－"), "-");
        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
    }
}

void MemoryWidget::widgetminusslot(int row, const QString str)
{
    if (str == QString())
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    else {
        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "-" + str);
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        qDebug() << formatResult;
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+")
                       .replace(QString::fromUtf8("－"), "-");
        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
    }
}

QString MemoryWidget::formatExpression(const QString &text)
{
    return QString(text)
           .replace(QString::fromUtf8("＋"), "+")
           .replace(QString::fromUtf8("－"), "-")
           .replace(QString::fromUtf8("×"), "*")
           .replace(QString::fromUtf8("÷"), "/")
           .replace(QString::fromUtf8(","), "");
}

MemoryWidget::~MemoryWidget()
{

}
