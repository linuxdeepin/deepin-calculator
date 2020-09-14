#include "programmodule.h"
#include <DIconButton>
#include <DLabel>
#include <DPushButton>

#include "src/widgets/probitwidget.h"

const int EXPRESSIONBAR_HEIGHT = 95;

ProgramModule::ProgramModule(QWidget *parent)
    : DWidget(parent)
    , m_proExpressionBar(new ProExpressionBar(this))
    , m_proListView(new ProListView(this))
    , m_proListModel(new ProListModel(this))
    , m_proListDelegate(new ProListDelegate())
    , m_checkBtnKeypad(new ProCheckBtnKeypad(this))
    , m_programmerKeypad(new ProgrammerKeypad)
    , m_proSystemKeypad(new ProSystemKeypad)
    , m_stackWidget(new QStackedWidget(this))
    , m_arrowRectangle(new DArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget/*, this*/))
    , m_arrowListWidget(new DListWidget)
    , m_programmerArrowDelegate(new ProgrammerArrowDelegate)
{
    m_proExpressionBar->setFixedHeight(EXPRESSIONBAR_HEIGHT);
    m_proListView->setModel(m_proListModel);
    m_proListView->setItemDelegate(m_proListDelegate);
    m_stackWidget->addWidget(m_programmerKeypad);
    m_stackWidget->addWidget(m_proSystemKeypad);
    m_stackWidget->setCurrentWidget(m_programmerKeypad);
    m_stackWidget->setFixedSize(451, 279);

    QVBoxLayout *vlay = new QVBoxLayout(this);
    vlay->addWidget(m_proExpressionBar);
    vlay->addWidget(m_proListView);
    vlay->addWidget(m_checkBtnKeypad);
    vlay->addWidget(m_stackWidget);
    vlay->setSpacing(0);
    vlay->setMargin(0);
    vlay->setContentsMargins(0, 0, 0, 0);

    /*
    m_arrowListWidget->setItemDelegate(m_programmerArrowDelegate);
    m_arrowListWidget->setFrameShape(QFrame::NoFrame); //设置边框类型，无边框
    m_arrowListWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    QListWidgetItem *item1 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget1 = new ProgrammerItemWidget("算数移位", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
    item1->setFlags(Qt::NoItemFlags);
    item1->setSizeHint(QSize(250, 34));
    m_arrowListWidget->insertItem(0, item1);
    m_arrowListWidget->setItemWidget(item1, itemwidget1);
    QListWidgetItem *item2 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget2 = new ProgrammerItemWidget("逻辑移位", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
    item2->setFlags(Qt::NoItemFlags);
    item2->setSizeHint(QSize(250, 34));
    m_arrowListWidget->insertItem(1, item2);
    m_arrowListWidget->setItemWidget(item2, itemwidget2);
    QListWidgetItem *item3 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget3 = new ProgrammerItemWidget("旋转循环移位", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
    item3->setFlags(Qt::NoItemFlags);
    item3->setSizeHint(QSize(250, 34));
    m_arrowListWidget->insertItem(2, item3);
    m_arrowListWidget->setItemWidget(item3, itemwidget3);
    QListWidgetItem *item4 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget4 = new ProgrammerItemWidget("循环移位旋转", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
    item4->setFlags(Qt::NoItemFlags);
    item4->setSizeHint(QSize(250, 34));
    m_arrowListWidget->insertItem(3, item4);
    m_arrowListWidget->setItemWidget(item4, itemwidget4);
    m_arrowListWidget->setFixedSize(QSize(250, 136));
    m_arrowRectangle->setRadius(15);
    m_arrowRectangle->setArrowWidth(48);
    m_arrowRectangle->setArrowHeight(21);
    m_arrowRectangle->setContent(m_arrowListWidget);
    m_arrowListWidget->move(m_arrowRectangle->pos().x(), m_arrowRectangle->pos().y() + m_arrowRectangle->arrowHeight() + 11);

    m_arrowRectangle->setWidth(250);
    m_arrowRectangle->setShadowXOffset(0);
    m_arrowRectangle->setShadowYOffset(0);
    m_arrowRectangle->setShadowBlurRadius(0);
    m_arrowRectangle->setHeight(181);

    connect(m_arrowListWidget, &QListWidget::itemClicked, [ = ](QListWidgetItem * item) {
        m_arrowListWidget->setCurrentItem(item);
    });
    connect(m_arrowListWidget, &QListWidget::currentItemChanged, [ = ](QListWidgetItem * current, QListWidgetItem * previous) {
        static_cast<ProgrammerItemWidget *>(m_arrowListWidget->itemWidget(current))->findChild<DIconButton *>("markBtn")->setHidden(false);
        if (m_arrowListWidget->row(previous) != -1) {
            static_cast<ProgrammerItemWidget *>(m_arrowListWidget->itemWidget(previous))->findChild<DIconButton *>("markBtn")->setHidden(true);
        }
    });
    */

//    ProListView *prolistview = new ProListView(this);
//    ProListModel *prolistmodel = new ProListModel(this);
//    ProListDelegate *prolistdelegate = new ProListDelegate(this);
//    prolistview->setModel(prolistmodel);
//    prolistview->setItemDelegate(prolistdelegate);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_checkBtnKeypad, &ProCheckBtnKeypad::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_programmerKeypad, &ProgrammerKeypad::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_proListDelegate, &ProListDelegate::setThemeType);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_proExpressionBar, &ProExpressionBar::initTheme);
}

ProgramModule::~ProgramModule()
{

}
