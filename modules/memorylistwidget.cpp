#include "memorylistwidget.h"

#include <QDebug>
#include <QFont>
#include <QModelIndex>
#include <QProxyStyle>
#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QEvent>

MemoryListWidget::MemoryListWidget(QWidget *parent)
    : QWidget(parent)
    , m_listwidget(new QListWidget(this))
    , m_pDeleteBtn(new QPushButton())
    , m_isempty(true)
{
    QPalette pal;
    pal.setColor(QPalette::Background, QColor(255, 255, 255));
    setAutoFillBackground(true);
    setPalette(pal);
    this->setContentsMargins(12, 0, 13, 0);
//    this->setFixedSize(344, 300);
//    m_pDeleteBtn->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/images/light/+_normal.svg);} \
//                                 QPushButton:hover {image:url(:/images/light/+_hover.svg);} \
//                                 QPushButton:pressed {image:url(:/images/light/+_press.svg);}");
    QVBoxLayout *lay = new QVBoxLayout(this);

    lay->setSpacing(0);
    lay->setMargin(0);
    lay->addWidget(m_listwidget);
//    lay->addLayout(layH);
    m_listwidget->setFixedHeight(200);
    m_listwidget->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listwidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_listwidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listwidget->setAutoScroll(false);
    m_listwidget->setSelectionRectVisible(false);
    m_listwidget->setFocusPolicy(Qt::NoFocus);
    m_listwidget->setStyleSheet("QListView::item:hover{color:black;background-color:rgb(180,180,180);} \
                                 QListView::item:pressed{color:black;background-color:rgb(115,115,115);} \
                                 QListView::item{color:black;background-color:rgb(150,150,150);} \
                                 QListView{color:black;background-color:rgb(150,150,150);}");
    lay->addStretch();
    this->setLayout(lay);
    connect(m_listwidget, &QListWidget::itemClicked, this, [ = ](QListWidgetItem * item) {

    });
//    connect(m_listwidget,&QListWidget::currentItemChanged,this,&Widget::on_listWidget_currentItemChanged);
}

void MemoryListWidget::generateData(const QString str)
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
    item1->setSizeHint(QSize(344, 100));
//    item1->setBackgroundColor(QColor(125,125,125,254));
//    item1->setFlags(Qt::NoItemFlags);
    MemoryItemWidget *widget = new MemoryItemWidget(this);
    widget->setFixedSize(344, 100);
    m_listwidget->insertItem(0, item1);
    m_listwidget->setItemWidget(item1, widget);
    connect(widget, &MemoryItemWidget::plusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryListWidget::widgetplus(row);
    });
    connect(widget, &MemoryItemWidget::minusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryListWidget::widgetminus(row);
    });
    connect(widget, &MemoryItemWidget::cleanbtnclicked, this, [ = ]() {
        m_listwidget->takeItem(m_listwidget->row(item1));
        if (m_listwidget->count() == 0) {
            m_listwidget->addItem("内存中没有内容");
            m_isempty = true;
            emit mListUnavailable();
        }
    });
}

void MemoryListWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOptionButton button;
    QRect widgetrect = QRect(this->rect());
    button.rect = QRect(widgetrect.right() - 40, widgetrect.bottom() - 40, 30, 30);
    button.state |= QStyle::State_Enabled;
    button.icon = QIcon(QString(":/images/light/+_normal.svg"));
    if (button.rect.contains(m_mousepoint)) {
        if (m_type == 0) {
            button.state |= QStyle::State_MouseOver;
            button.icon = QIcon(QString(":/images/light/+_hover.svg"));
        } else if (m_type == 1) {
            button.state |= QStyle::State_Sunken;
            button.icon = QIcon(QString(":/images/light/+_press.svg"));
        } else {
            button.state |= QStyle::State_Enabled;
            button.icon = QIcon(QString(":/images/light/+_normal.svg"));
        }
    }
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, &painter);
    QWidget::paintEvent(event);
}

void MemoryListWidget::mousePressEvent(QMouseEvent *event)
{
    m_type = -1;
    QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
    m_mousepoint = pEvent->pos();
    QStyleOptionButton button;
    QRect widgetrect = QRect(this->rect());
    button.rect = QRect(widgetrect.right() - 40, widgetrect.bottom() - 40, 30, 30);
    if (button.rect.contains(m_mousepoint)) {
        switch (event->type()) {
        case QEvent::MouseMove:
            m_type = 0;
            break;
        case QEvent::MouseButtonPress:
            m_type = 1;
            break;
        default:
            break;
        }
        update();
        m_listwidget->clear();
        m_listwidget->addItem("内存中没有内容");
        m_isempty = true;
        emit mListUnavailable();
    }

    QRect rect = this->frameGeometry();
    if (rect.contains(m_mousepoint) == true)
        emit insidewidget();
    QWidget::mousePressEvent(event);
}

void MemoryListWidget::memoryplus(const QString str)
{
    QString str1 = (str == QString()) ? "0" : str;
    if (m_isempty == false) {
        m_listwidget->item(0)->setData(Qt::DisplayRole, m_listwidget->item(0)->data(Qt::EditRole).toInt() + str1.toInt());
    } else {
        m_listwidget->clear();
        generateData(str1);
    }
}

void MemoryListWidget::memoryminus(const QString str)
{
    QString str1 = (str == QString()) ? "0" : str;
    if (m_isempty == false) {
        m_listwidget->item(0)->setData(Qt::DisplayRole, m_listwidget->item(0)->data(Qt::EditRole).toInt() - str1.toInt());
    } else {
        m_listwidget->clear();
        generateData(QString());
        m_listwidget->item(0)->setData(Qt::DisplayRole, 0 - str1.toInt());
    }
}

void MemoryListWidget::memoryclean()
{
    m_listwidget->clear();
    m_isempty = true;
    emit mListUnavailable();
}

QString MemoryListWidget::getfirstnumber()
{
    if (m_isempty == false) {
        return m_listwidget->item(0)->data(Qt::EditRole).toString();
    } else {
        return QString();
    }
}

void MemoryListWidget::widgetplusslot(int row, const QString str)
{
    if (str == QString())
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole).toInt() + 0);
    else
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole).toInt() + str.toInt());
}

void MemoryListWidget::widgetminusslot(int row, const QString str)
{
    if (str == QString())
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole).toInt() - 0);
    else
        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole).toInt() - str.toInt());
}

//bool MemoryListWidget::event(QEvent *event)
//{
//    if (event->type() == QEvent::ActivationChange) {
//        qDebug() << "out!";
//        if (QApplication::activePopupWidget() != this) {
//            emit outofwidget();
//        }
//    }
//    return QWidget::event(event);
//}

MemoryListWidget::~MemoryListWidget()
{

}
