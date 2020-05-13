#include "scihistorywidget.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include "dthememanager.h"
#include "utils.h"
#include "../math/quantity.h"

SciHistoryWidget::SciHistoryWidget(QWidget *parent)
    : DWidget(parent)
    , m_memorywidget(new MemoryWidget(this))
    , m_listView(new SimpleListView(1))
    , m_listDelegate(new SimpleListDelegate(1, this))
    , m_listModel(new SimpleListModel(this))
    , m_buttonbox(new DButtonBox(this))
    , m_clearbutton(new DIconButton(this))
{
    m_stacklayout = new QStackedLayout();
    QVBoxLayout *m_Vlayout = new QVBoxLayout(this);
    QHBoxLayout *m_Hlayout = new QHBoxLayout();
    m_Hlayout->addSpacing(70);
    m_Hlayout->addWidget(m_buttonbox);
    m_buttonbox->setFixedWidth(160);
    m_Hlayout->addSpacing(30);
    m_Hlayout->addWidget(m_clearbutton);
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
}

SciHistoryWidget::~SciHistoryWidget() {}
