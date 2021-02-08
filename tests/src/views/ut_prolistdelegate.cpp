#include "ut_prolistdelegate.h"

Ut_ProListDelegate::Ut_ProListDelegate()
{

}

TEST_F(Ut_ProListDelegate, setThemeType)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    m_proListDelegate->setThemeType(1);
    ASSERT_EQ(m_proListDelegate->m_themeType, 1);
}

TEST_F(Ut_ProListDelegate, currentfocusindex)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    m_proListDelegate->currentfocusindex(QModelIndex());
    ASSERT_EQ(m_proListDelegate->m_focusindex, QModelIndex());
}

TEST_F(Ut_ProListDelegate, sizeHint)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    ProListModel *model = new ProListModel();
    m_proListDelegate->sizeHint(QStyleOptionViewItem(), model->index(0, 0));
    ASSERT_EQ(m_proListDelegate->sizeHint(QStyleOptionViewItem(), model->index(3, 0)), QSize(451, 28));
}

TEST_F(Ut_ProListDelegate, editorEvent)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    ProListModel *model = new ProListModel();
    QStyleOptionViewItem item;
    ASSERT_TRUE(m_proListDelegate->editorEvent(new QEvent(QEvent::Type::None), model, item, model->index(0, 0)));
}

//bool stub_focus_prolistdelegateT()
//{
//    return true;
//}

bool stub_focus_prolistdelegateF()
{
    return false;
}

QModelIndex stub_index_prolistdelegate1()
{
    ProListModel *m_model = new ProListModel();
    return m_model->index(0, 0, QModelIndex());
}

QModelIndex stub_index_prolistdelegate3()
{
    ProListModel *m_model = new ProListModel();
    return m_model->index(3, 0, QModelIndex());
}

TEST_F(Ut_ProListDelegate, paintnofocus)
{
    m_prolistview1 = new ProListView();
    m_model1 = new ProListModel();
    m_proListDelegate1 = new ProListDelegate();
    m_prolistview1->setModel(m_model1);
    m_prolistview1->setItemDelegate(m_proListDelegate1);
    QPainter *painter = new QPainter();
    Stub stub;
    stub.set(ADDR(ProListView, hasFocus), stub_focus_prolistdelegateF);
    QStyleOptionViewItem option;
    Stub stub1;
    stub1.set(ADDR(ProListView, currentIndex), stub_index_prolistdelegate3);
    m_proListDelegate1->m_themeType = 0;
    m_prolistview1->setCurrentIndex(m_model1->index(3, 0, QModelIndex()));
    m_proListDelegate1->paint(painter, option, m_model1->index(3, 0, QModelIndex()));
    Stub stub2;
    stub2.set(ADDR(ProListView, currentIndex), stub_index_prolistdelegate1);
    m_proListDelegate1->m_themeType = 1;
    m_prolistview1->setCurrentIndex(m_model1->index(0, 0, QModelIndex()));
    m_proListDelegate1->paint(painter, option, m_model1->index(0, 0, QModelIndex()));
}
