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
#include <DGuiApplicationHelper>
#include "../utils.h"

MemoryWidget::MemoryWidget(QWidget *parent)
    : QWidget(parent)
    , m_listwidget(new MemoryListWidget(this))
    , m_clearbutton(new DPushButton(this))
    , m_isempty(true)
{
    m_evaluator = Evaluator::instance();
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *lay = new QVBoxLayout(this);
    QHBoxLayout *layH = new QHBoxLayout();

    lay->setSpacing(0);
    lay->setMargin(0);
    lay->addWidget(m_listwidget);

    m_listwidget->setFrameShape(QFrame::NoFrame);
    m_listwidget->setFixedHeight(260);
    m_listwidget->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listwidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_listwidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listwidget->setAutoScroll(false);
    m_listwidget->setSelectionRectVisible(false);
    m_listwidget->setFocusPolicy(Qt::NoFocus);
//    m_listwidget->setWordWrap(true);
//    m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:transparent;} \
//                                 QListWidget::item:hover{color:black;background-color:rgba(0,0,0,0.1 * 255);} \
//                                 QListWidget{color:black;background-color:transparent;}");
    lay->addStretch();
    layH->addStretch();
    layH->addWidget(m_clearbutton);
    m_clearbutton->setFixedSize(40, 40);
//    m_clearbutton->setStyleSheet("QPushButton {border:none;background-color: transparent;image:url(:/images/light/clear_normal.svg);} \
//                                  QPushButton:hover {border: 1px solid #000000;image:url(:/images/light/clear_hover.svg);} \
//                                  QPushButton:pressed {image:url(:/images/light/clear_press.svg);}");
    connect(m_clearbutton, &DPushButton::clicked, this, [ = ]() {
        m_listwidget->clear();
        m_listwidget->addItem("内存中没有内容");
        m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
        m_isempty = true;
        list.clear();
        emit mListUnavailable();
    });
    lay->addLayout(layH);
    this->setLayout(lay);
    connect(m_listwidget, &QListWidget::itemClicked, this, [ = ](QListWidgetItem * item) {
        QString str = item->data(Qt::DisplayRole).toString();
        if (item->flags() != Qt::NoItemFlags)
            emit itemclick(str);
    });
//    connect(m_listwidget, &QListWidget::itemPressed, this, [ = ](QListWidgetItem * item) {
//        m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:transparent;} \
//                                     QListWidget::item:selected{color:#FFFFFF;background-color:#0081FF;} \
//                                     QListWidget{color:black;background-color:transparent;}");
//    });
}

void MemoryWidget::generateData(Quantity answer)
{
    //500 memory number limit
    if (list.count() == 500) {
        list.pop_back();
        m_listwidget->takeItem(499);
    }
    if (m_isempty == true) {
        m_listwidget->clear();
    }
    m_isempty = false;
    emit mListAvailable();
    QListWidgetItem *item1 = new QListWidgetItem();
    if (answer == Quantity(0)) {
        item1->setData(Qt::DisplayRole, "0");
    } else {
        const QString result = DMath::format(answer, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace('-', "－").replace('+', "＋");
        item1->setData(Qt::DisplayRole, formatResult);
    }
    list.insert(0, answer); //对于新增数据，同步在list中加入对应的Quantity
    item1->setTextAlignment(Qt::AlignRight | Qt::AlignTop);
    QFont font;
    font.setPixelSize(30);
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
        list.removeAt(m_listwidget->row(item1));
        m_listwidget->takeItem(m_listwidget->row(item1));
        if (m_listwidget->count() == 0) {
            m_listwidget->addItem("内存中没有内容");
            m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
            m_isempty = true;
            emit mListUnavailable();
        }
    });
    widget->themetypechanged(m_themetype);
    connect(this, &MemoryWidget::themechange, widget, &MemoryItemWidget::themetypechanged);
    connect(widget, &MemoryItemWidget::itemchanged, this, [ = ](int type) {
        if (type == 1) {
            m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:transparent;} \
                                         QListWidget::item:hover{color:black;background-color:rgba(0,0,0,0.1 * 255);} \
                                         QListWidget{color:black;background-color:transparent;}");
        } else {
            m_listwidget->setStyleSheet("QListWidget::item{color:#B4B4B4;background-color:transparent;} \
                                         QListWidget::item:hover{color:#B4B4B4;background-color:rgba(255,255,255,0.05);} \
                                         QListWidget{color:#B4B4B4;background-color:transparent;}");
        }
    });
}

void MemoryWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (m_themetype == 1) {
        QPen pen(QBrush(QColor("#F8F8F8")), 0);
        painter.setPen(pen);
    } else {
        QPen pen(QBrush(QColor("#252525")), 0);
        painter.setPen(pen);
    }
//    painter.setBrush(QBrush(QColor("#F8F8F8")));
    QRect rect(this->rect().left(), this->rect().top(), this->rect().width(), this->rect().bottom() + 30);
    painter.drawRect(rect);
    QWidget::paintEvent(event);
}

void MemoryWidget::mousePressEvent(QMouseEvent *event)
{
    m_type = -1;
    QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
    m_mousepoint = pEvent->pos();

    QRect rect(this->frameGeometry());
    if (rect.contains(m_mousepoint) == true)
        emit insidewidget();
    QWidget::mousePressEvent(event);
}

void MemoryWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_listwidget->itemAt(event->pos()) && !m_isempty) {
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
            QClipboard *clipboard = QApplication::clipboard();
            QString originalText = clipboard->text();
            clipboard->setText(m_listwidget->itemAt(event->pos())->data(Qt::EditRole).toString().remove("\n"));
        });
        connect(clean, &QAction::triggered, this, [ = ]() {
            m_listwidget->takeItem(m_listwidget->row(m_listwidget->itemAt(event->pos())));
            if (m_listwidget->count() == 0) {
                m_listwidget->addItem("内存中没有内容");
                m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
                m_isempty = true;
                list.clear();
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

void MemoryWidget::memoryplus(Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (m_isempty == false) {
//        QString exp = QString(m_listwidget->item(0)->data(Qt::EditRole).toString() + "+(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(0), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "+(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        list.replace(0, ans);
    } else {
        m_listwidget->clear();
        generateData(answer);
    }
}

void MemoryWidget::memoryminus(Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (m_isempty == false) {
//        QString exp = QString(m_listwidget->item(0)->data(Qt::EditRole).toString() + "-(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(0), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        list.replace(0, ans);
    } else {
        m_listwidget->clear();
        generateData(Quantity(0));
        QString exp = QString("0-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        list.replace(0, ans);
    }
}

void MemoryWidget::memoryclean()
{
    m_listwidget->clear();
    m_isempty = true;
    list.clear();
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

void MemoryWidget::widgetplusslot(int row, Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (answer == Quantity(0)) {
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    } else {
//        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "+(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(row), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "+(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
        list.replace(row, ans);
    }
}

void MemoryWidget::widgetminusslot(int row, Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(78));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (answer == Quantity(0))
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    else {
//        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "-(" + formatResultmem + ")");
        QString exp = QString(DMath::format(list.value(row), Quantity::Format::Fixed() + Quantity::Format::Precision(78)) + "-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
        list.replace(row, ans);
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

QString MemoryWidget::setitemwordwrap(const QString &text)
{
    QString result = text;
    result.replace('-', "－").replace('+', "＋");
    int index = result.indexOf("e");
    if (index > 0 && result.left(index).length() > 13) {
        result.insert(index, "\n");
    } else if (index <= 0 && result.length() > 21) {
        result.insert(20, "\n");
    }
    return result;
}

void MemoryWidget::setThemeType(int type)
{
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    m_themetype = typeIn;
    emit themechange(m_themetype);
    if (m_themetype == 1) {
        m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:transparent;} \
                                     QListWidget::item:hover{color:black;background-color:rgba(0,0,0,0.1 * 255);} \
                                     QListWidget{color:black;background-color:transparent;}");
        m_clearbutton->setStyleSheet("QPushButton {border:none;background-color: transparent;image:url(:/images/light/empty_normal.svg);} \
                                      QPushButton:hover {image:url(:/images/light/empty_hover.svg);} \
                                      QPushButton:pressed {image:url(:/images/light/empty_press.svg);}");
        connect(m_listwidget, &QListWidget::itemPressed, this, [ = ](QListWidgetItem * item) {
            m_listwidget->setStyleSheet("QListWidget::item{color:black;background-color:transparent;} \
                                         QListWidget::item:selected{color:#FFFFFF;background-color:#0081FF;} \
                                         QListWidget{color:black;background-color:transparent;}");
        });
    } else {
        m_listwidget->setStyleSheet("QListWidget::item{color:#B4B4B4;background-color:transparent;} \
                                     QListWidget::item:hover{color:#B4B4B4;background-color:rgba(255,255,255,0.05);} \
                                     QListWidget{color:#B4B4B4;background-color:transparent;}");
        m_clearbutton->setStyleSheet("QPushButton {border:none;background-color: transparent;image:url(:/images/dark/empty_normal.svg);} \
                                      QPushButton:hover {image:url(:/images/dark/empty_hover.svg);} \
                                      QPushButton:pressed {image:url(:/images/dark/empty_press.svg);}");
        connect(m_listwidget, &QListWidget::itemPressed, this, [ = ](QListWidgetItem * item) {
            m_listwidget->setStyleSheet("QListWidget::item{color:#B4B4B4;background-color:transparent;} \
                                         QListWidget::item:selected{color:#FFFFFF;background-color:#0059D2;} \
                                         QListWidget{color:#B4B4B4;background-color:transparent;}");
        });
    }

}

MemoryWidget::~MemoryWidget()
{

}
