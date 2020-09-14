#include "proexpressionbar.h"

#include <QVBoxLayout>

const int LIST_HEIGHT = 35; //输入栏上方表达式的高度
const int INPUTEDIT_HEIGHT = 55;

ProExpressionBar::ProExpressionBar(QWidget *parent)
    : DWidget(parent)
{
    m_listView = new SimpleListView;
    m_listDelegate = new SimpleListDelegate(0, this);
    m_listModel = new SimpleListModel(0, this);
    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);
    m_inputEdit = new InputEdit;
    m_listView->setFixedHeight(LIST_HEIGHT);
    m_inputEdit->setFixedHeight(INPUTEDIT_HEIGHT);
    m_inputEdit->setAlignment(Qt::AlignRight);
    m_inputEdit->setTextMargins(10, 0, 10, 6);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_listView);
    layout->addWidget(m_inputEdit);
    layout->setMargin(0);
    layout->setSpacing(0);
}

ProExpressionBar::~ProExpressionBar()
{

}

void ProExpressionBar::initTheme(int type)
{
    m_listDelegate->setThemeType(type);
    m_inputEdit->themetypechanged(type);
}
