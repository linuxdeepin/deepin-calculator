#include "programmodule.h"
#include <DIconButton>
#include <DLabel>
#include <DPushButton>

ProgramModule::ProgramModule(QWidget *parent)
    : DWidget(parent)
    , m_arrowRectangle(new DArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget, this))
    , m_arrowListWidget(new DListWidget)
    , m_programmerArrowDelegate(new ProgrammerArrowDelegate)
{
//    m_arrowListWidget->setItemDelegate(m_programmerArrowDelegate);
//    m_arrowListWidget->setFrameShape(QFrame::NoFrame); //设置边框类型，无边框
//    QListWidgetItem *item1 = new QListWidgetItem();
//    ProgrammerItemWidget *itemwidget1 = new ProgrammerItemWidget("算数移位", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
//    item1->setFlags(Qt::NoItemFlags);
//    item1->setSizeHint(QSize(220, 50));
//    m_arrowListWidget->insertItem(0, item1);
//    m_arrowListWidget->setItemWidget(item1, itemwidget1);
//    QListWidgetItem *item2 = new QListWidgetItem();
//    ProgrammerItemWidget *itemwidget2 = new ProgrammerItemWidget("逻辑移位", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
//    item2->setFlags(Qt::NoItemFlags);
//    item2->setSizeHint(QSize(220, 50));
//    m_arrowListWidget->insertItem(1, item2);
//    m_arrowListWidget->setItemWidget(item2, itemwidget2);
//    QListWidgetItem *item3 = new QListWidgetItem();
//    ProgrammerItemWidget *itemwidget3 = new ProgrammerItemWidget("旋转循环移位", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
//    item3->setFlags(Qt::NoItemFlags);
//    item3->setSizeHint(QSize(220, 50));
//    m_arrowListWidget->insertItem(2, item3);
//    m_arrowListWidget->setItemWidget(item3, itemwidget3);
//    QListWidgetItem *item4 = new QListWidgetItem();
//    ProgrammerItemWidget *itemwidget4 = new ProgrammerItemWidget("循环移位旋转", QIcon::fromTheme(":/assets/images/deepin-calculator.svg"));
//    item4->setFlags(Qt::NoItemFlags);
//    item4->setSizeHint(QSize(220, 50));
//    m_arrowListWidget->insertItem(3, item4);
//    m_arrowListWidget->setItemWidget(item4, itemwidget4);
//    m_arrowListWidget->setFixedSize(QSize(220, 200));
//    m_arrowRectangle->setContent(m_arrowListWidget);
//    m_arrowListWidget->move(m_arrowRectangle->pos().x(), m_arrowRectangle->pos().y() + m_arrowRectangle->arrowHeight());

//    m_arrowRectangle->setWidth(220);
//    m_arrowRectangle->setShadowXOffset(0);
//    m_arrowRectangle->setShadowYOffset(0);
//    m_arrowRectangle->setShadowBlurRadius(0);
//    m_arrowRectangle->setHeight(200 + m_arrowRectangle->arrowHeight());

//    BitButton *bitbtn = new BitButton(this);
}

ProgramModule::~ProgramModule()
{

}
