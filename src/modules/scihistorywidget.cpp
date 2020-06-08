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
//    , m_memorywidget(new MemoryWidget(1, this))
    , m_listView(new SimpleListView(1))
    , m_listDelegate(new SimpleListDelegate(1, this))
    , m_listModel(new SimpleListModel(this))
    , m_buttonbox(new DButtonBox(this))
    , m_clearbutton(new IconButton(this, true))
    , m_clearbuttonM(new IconButton(this, true))
    , m_buttonstack(new QStackedWidget(this))
{
    memoryPublic = MemoryPublic::instance(this);
    m_memorywidget = memoryPublic->getwidget(MemoryPublic::scientificright);
    m_stacklayout = new QStackedLayout();
    QVBoxLayout *m_Vlayout = new QVBoxLayout(this);
    QHBoxLayout *m_Hlayout = new QHBoxLayout();
    m_Hlayout->addSpacing(70);
    m_Hlayout->addWidget(m_buttonbox, 0, Qt::AlignCenter);
    m_buttonbox->setFixedWidth(160);
    m_Hlayout->addSpacing(30);

    //button stack
    m_Hlayout->addWidget(m_buttonstack);
    m_buttonstack->setFixedSize(40, 40);
    m_buttonstack->addWidget(m_clearbutton);
    m_buttonstack->addWidget(m_clearbuttonM);
    m_buttonstack->setCurrentIndex(0);
    m_clearbutton->settooltip(false);
    m_clearbuttonM->settooltip(true);
    m_clearbutton->setHidden(!(ishideH & indexH));
    m_clearbuttonM->setHidden(!(ishideM & indexM));

    m_Hlayout->addSpacing(20);
    m_Vlayout->addLayout(m_Hlayout);

    //mem & his stack
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
    DButtonBoxButton *historybtn = new DButtonBoxButton(tr("History"));
    historybtn->setFixedWidth(80);
    DButtonBoxButton *memorybtn = new DButtonBoxButton(tr("Memory"));
    memorybtn->setFixedWidth(80);
    listBtnBox << historybtn << memorybtn;
    m_buttonbox->setButtonList(listBtnBox, true);
    m_buttonbox->setId(historybtn, 0);
    m_buttonbox->setId(memorybtn, 1);
    connect(m_buttonbox->button(0), &QAbstractButton::clicked, this, [ = ]() {
        indexH = true;
        indexM = false;
        m_stacklayout->setCurrentIndex(0);
        m_buttonstack->setCurrentIndex(0);
        m_clearbutton->settooltip(false);
        m_clearbutton->setHidden(!(ishideH & indexH));
    });
    connect(m_buttonbox->button(1), &QAbstractButton::clicked, this, [ = ]() {
        indexH = false;
        indexM = true;
        bool b = m_clearbuttonM->isHidden();
        m_stacklayout->setCurrentIndex(1);
        m_buttonstack->setCurrentIndex(1);
        m_clearbuttonM->settooltip(true);
        m_clearbuttonM->setHidden(!(ishideM & indexM));
    });
    connect(m_clearbutton, &IconButton::clicked, this, [ = ]() {
        m_listModel->clearItems();
        ishideH = false;
        m_clearbutton->setHidden(!(ishideH & indexH));
        setFocus();
    });
    connect(m_clearbuttonM, &IconButton::clicked, this, [ = ]() {
        memoryPublic->memoryclean();
        setFocus();
    });
    connect(memoryPublic, &MemoryPublic::filledMem, this, [ = ]() {
        ishideM = true;
        m_clearbuttonM->setHidden(!(ishideM & indexM));
    });
    connect(memoryPublic, &MemoryPublic::emptyMem, this, [ = ]() {
        ishideM = false;
        m_clearbuttonM->setHidden(!(ishideM & indexM));
    });
}

SciHistoryWidget::~SciHistoryWidget() {}

void SciHistoryWidget::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    QPen pen(QBrush(QColor("#F8F8F8")), 0);
//    painter.setPen(pen);

//    QRect rect(this->rect().left(), this->rect().top(), this->rect().width(), this->rect().bottom() + 30);
//    painter.drawRect(rect);
    //    QWidget::paintEvent(event);
}

void SciHistoryWidget::focusOnButtonbox()
{
    if (m_stacklayout->currentIndex() == 0) {
        m_buttonbox->button(0)->setChecked(true);
    } else {
        m_buttonbox->button(1)->setChecked(true);
    }
}

MemoryWidget *SciHistoryWidget::getMemoryWidget()
{
    return m_memorywidget;
}

void SciHistoryWidget::memoryFunctions(int row, Quantity answer, SciHistoryWidget::memOperate operate)
{
    switch (operate) {
    case generateData:
        memoryPublic->generateData(answer);
        break;
    case memoryplus:
        memoryPublic->memoryplus(answer);
        break;
    case memoryminus:
        memoryPublic->memoryminus(answer);
        break;
    case memoryclean:
        memoryPublic->memoryclean();
        break;
    case widgetplus:
        memoryPublic->widgetplus(row, answer);
        break;
    case widgetminus:
        memoryPublic->widgetminus(row, answer);
        break;
    }
}

void SciHistoryWidget::themeChanged(int type)
{
    QString path;
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    if (typeIn == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
    m_clearbuttonM->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
    memoryPublic->setThemeType(typeIn);
    m_listDelegate->setThemeType(typeIn);
}

void SciHistoryWidget::ishistoryfilled(bool b)
{
    ishideH = b;
    m_clearbutton->setHidden(!(ishideH & indexH));
}
