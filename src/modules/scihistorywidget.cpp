#include "scihistorywidget.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include "dthememanager.h"
#include "../utils.h"
#include "../math/quantity.h"

SciHistoryWidget::SciHistoryWidget(QWidget *parent)
    : DWidget(parent)
    , m_memorywidget(new MemoryWidget(1, this))
    , m_listView(new SimpleListView(1))
    , m_listDelegate(new SimpleListDelegate(1, this))
    , m_listModel(new SimpleListModel(this))
    , m_buttonbox(new DButtonBox(this))
    , m_clearbutton(new IconButton(this, true))
{
    m_stacklayout = new QStackedLayout();
    QVBoxLayout *m_Vlayout = new QVBoxLayout(this);
    QHBoxLayout *m_Hlayout = new QHBoxLayout();
    m_Hlayout->addSpacing(70);
    m_Hlayout->addWidget(m_buttonbox);
    m_buttonbox->setFixedWidth(160);
    m_Hlayout->addSpacing(30);
    m_Hlayout->addWidget(m_clearbutton);
//    QString path;
//    path = QString(":/assets/images/%1/").arg("light");
//    m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
    m_Hlayout->addSpacing(20);
    m_Vlayout->addLayout(m_Hlayout);
    m_Vlayout->addLayout(m_stacklayout);
    m_stacklayout->addWidget(m_listView);
    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);
    m_stacklayout->addWidget(m_memorywidget);
    m_stacklayout->setCurrentIndex(0);

    //    m_Vlayout->setSpacing(0);
    m_Vlayout->setMargin(0);
    m_Vlayout->setContentsMargins(0, 0, 0, 11);
    this->setLayout(m_Vlayout);
    this->setFixedWidth(370);

    QList<DButtonBoxButton *> listBtnBox;
    DButtonBoxButton *historybtn = new DButtonBoxButton(QStringLiteral("历史记录"));
    historybtn->setFixedWidth(80);
    DButtonBoxButton *memorybtn = new DButtonBoxButton(QStringLiteral("内存"));
    memorybtn->setFixedWidth(80);
    listBtnBox << historybtn << memorybtn;
    m_buttonbox->setButtonList(listBtnBox, true);
    m_buttonbox->setId(historybtn, 0);
    m_buttonbox->setId(memorybtn, 1);
    connect(m_buttonbox->button(0), &QAbstractButton::clicked, this, [ = ]() {
        m_stacklayout->setCurrentIndex(0);
    });
    connect(m_buttonbox->button(1), &QAbstractButton::clicked, this, [ = ]() {
        m_stacklayout->setCurrentIndex(1);
    });
//    this->setStyleSheet("border:1px solid #000000;background-color:transparent;");
}

SciHistoryWidget::~SciHistoryWidget() {}

void SciHistoryWidget::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    QPen pen(QBrush(QColor("#F8F8F8")), 0);
//    painter.setPen(pen);

////    painter.setBrush(QBrush(QColor("#F8F8F8")));
//    QRect rect(this->rect().left(), this->rect().top(), this->rect().width(), this->rect().bottom() + 30);
//    painter.drawRect(rect);
    //    QWidget::paintEvent(event);
}

void SciHistoryWidget::focusOnButtonbox()
{
    if (m_stacklayout->currentIndex() == 0) {
        emit m_buttonbox->button(0)->clicked();
    } else {
        emit m_buttonbox->button(1)->clicked();
    }
}

void SciHistoryWidget::themeChanged(int type)
{
    QString path;
    if (type == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
}
