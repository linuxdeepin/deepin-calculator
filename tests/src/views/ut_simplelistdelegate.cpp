#include "ut_simplelistdelegate.h"

#include "../../src/views/simplelistdelegate.h"
#include "../../src/views/simplelistmodel.h"
#include "../../src/views/simplelistview.h"
#include <QPainter>

Ut_SimpleListDelegate::Ut_SimpleListDelegate()
{

}

TEST_F(Ut_SimpleListDelegate, setHisLink)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setHisLink(0);
    ASSERT_EQ(m_simpleListDelegate->m_linkItem.count(), 1);
}

TEST_F(Ut_SimpleListDelegate, setHisLinked)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setHisLinked(0);
    ASSERT_EQ(m_simpleListDelegate->m_linkedIten.count(), 1);
}

TEST_F(Ut_SimpleListDelegate, removeLine2)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setHisLink(0);
    m_simpleListDelegate->setHisLinked(0);
    m_simpleListDelegate->removeLine(0, 0);
    ASSERT_EQ(m_simpleListDelegate->m_linkedIten.count(), 0);
}

TEST_F(Ut_SimpleListDelegate, removeLine1)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setHisLink(0);
    m_simpleListDelegate->setHisLinked(0);
    m_simpleListDelegate->setHisLink(1);
    m_simpleListDelegate->setHisLinked(1);
    m_simpleListDelegate->removeLine(0, 0);
    ASSERT_EQ(m_simpleListDelegate->m_linkedIten.count(), 1);
}

TEST_F(Ut_SimpleListDelegate, removeHisLink)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setHisLink(0);
    m_simpleListDelegate->setHisLink(1);
    m_simpleListDelegate->removeHisLink();
    ASSERT_EQ(m_simpleListDelegate->m_linkItem.count(), 1);
}

TEST_F(Ut_SimpleListDelegate, removeAllLink)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setHisLink(0);
    m_simpleListDelegate->setHisLinked(0);
    m_simpleListDelegate->setHisLink(1);
    m_simpleListDelegate->setHisLinked(1);
    m_simpleListDelegate->removeAllLink();
    ASSERT_EQ(m_simpleListDelegate->m_linkedIten.count(), 0);
}

TEST_F(Ut_SimpleListDelegate, removeHisLinked)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setHisLinked(0);
    m_simpleListDelegate->setHisLinked(1);
    m_simpleListDelegate->removeHisLinked();
    ASSERT_EQ(m_simpleListDelegate->m_linkedIten.count(), 1);
}

TEST_F(Ut_SimpleListDelegate, setThemeType)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setThemeType(1);
    ASSERT_EQ(m_simpleListDelegate->m_type, 1);
}

TEST_F(Ut_SimpleListDelegate, paintback)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->paintback(QModelIndex(), 1);
    ASSERT_EQ(m_simpleListDelegate->m_state, 1);
}

TEST_F(Ut_SimpleListDelegate, sizeHint)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->m_mode = 0;
    m_simpleListDelegate->sizeHint(QStyleOptionViewItem(), QModelIndex());
    m_simpleListDelegate->m_mode = 1;
    SimpleListModel *model = new SimpleListModel();
    m_simpleListDelegate->sizeHint(QStyleOptionViewItem(), model->index(0, 0));
    model->appendText("1＋1＝2", true);
    ASSERT_EQ(m_simpleListDelegate->sizeHint(QStyleOptionViewItem(), model->index(0, 0)).width(), 451);
}

//TEST_F(Ut_SimpleListDelegate, editorEvent)
//{
//    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
//    m_simpleListDelegate->m_mode = 0;
//    SimpleListModel *model = new SimpleListModel();
//    QStyleOptionViewItem item;
//    ASSERT_TRUE(m_simpleListDelegate->editorEvent(new QEvent(QEvent::Type::None), model, item, model->index(0, 0)));
//}

TEST_F(Ut_SimpleListDelegate, cutApart)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    QString linkNum = QString();
    QString expStr = QString();
    m_simpleListDelegate->cutApart("", linkNum, expStr);
    m_simpleListDelegate->cutApart("(－1)＋1＝0", linkNum, expStr);
    m_simpleListDelegate->cutApart("－(1)＋1＝0", linkNum, expStr);
    m_simpleListDelegate->cutApart("1E0＋1＝0", linkNum, expStr);
    linkNum = QString();
    expStr = QString();
    m_simpleListDelegate->cutApart("1＋2＝3", linkNum, expStr);
    ASSERT_EQ(linkNum, "1");
}

TEST_F(Ut_SimpleListDelegate, setSelect)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->setSelect(true);
    ASSERT_TRUE(m_simpleListDelegate->m_selected);
}

//bool stub_focus_simpledelegateT()
//{
//    return true;
//}

bool stub_focus_simpledelegateF()
{
    return false;
}

TEST_F(Ut_SimpleListDelegate, paintnofocus)
{
    SimpleListView *m_simplelistview = new SimpleListView(0);
    SimpleListModel *m_model = new SimpleListModel(0);
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simplelistview->setModel(m_model);
    m_simplelistview->setItemDelegate(m_simpleListDelegate);
    m_model->m_expressionList.append("1");
    m_model->updataList("2＝2", 0, false);
    QPainter *painter = new QPainter();
    Stub stub;
    stub.set(ADDR(SimpleListView, hasFocus), stub_focus_simpledelegateF);
    QStyleOptionViewItem option;
    QWidget wid;
    wid.setFixedWidth(10);
    option.widget = &wid;
    m_simpleListDelegate->m_type = 0;
    m_simpleListDelegate->paint(painter, option, QModelIndex());
    m_simpleListDelegate->m_type = 1;
    m_simpleListDelegate->paint(painter, option, m_model->index(0, 0, QModelIndex()));
}

TEST_F(Ut_SimpleListDelegate, paint1nofocus)
{
    SimpleListView *m_simplelistview = new SimpleListView(1);
    SimpleListModel *m_model = new SimpleListModel(1);
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(1);
    m_simplelistview->setModel(m_model);
    m_simplelistview->setItemDelegate(m_simpleListDelegate);
    m_model->m_expressionList.append("1");
    m_model->updataList("2＝2", 0, false);
    QPainter *painter = new QPainter();
    Stub stub;
    stub.set(ADDR(SimpleListView, hasFocus), stub_focus_simpledelegateF);
    QStyleOptionViewItem option;
    QWidget wid;
    wid.setFixedWidth(10);
    option.widget = &wid;
    m_simpleListDelegate->m_type = 0;
    m_simpleListDelegate->paint(painter, option, QModelIndex());
    m_simpleListDelegate->m_type = 1;
    m_simpleListDelegate->paint(painter, option, m_model->index(0, 0, QModelIndex()));
}

TEST_F(Ut_SimpleListDelegate, updateEditorGeometry)
{
    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
    m_simpleListDelegate->updateEditorGeometry(new QWidget(), QStyleOptionViewItem(), QModelIndex());
}

//QRect stub_visualRect_simpledelegate(void *obj, const QModelIndex &index)
//{
//    Q_UNUSED(index);
//    SimpleListView *o = (SimpleListView *)obj;
//    qDebug() << "visual";
//    return QRect(10, 10, 10, 10);
//}

//TEST_F(Ut_SimpleListDelegate, paintfocus)
//{
//    SimpleListView *m_simplelistview = new SimpleListView(0);
//    SimpleListModel *m_model = new SimpleListModel(0);
//    SimpleListDelegate *m_simpleListDelegate = new SimpleListDelegate(0);
//    m_simplelistview->setModel(m_model);
//    m_simplelistview->setItemDelegate(m_simpleListDelegate);
//    m_model->m_expressionList.append("1");
//    m_model->updataList("2", 0, false);
//    QPainter *painter = new QPainter();
//    Stub stub;
//    stub.set(ADDR(SimpleListView, hasFocus), stub_focus_simpledelegateT);
//    QStyleOptionViewItem option;
//    QWidget wid;
//    wid.setFixedWidth(10);
//    option.widget = &wid;
//    m_simpleListDelegate->m_focusindex = m_model->index(0, 0, QModelIndex());
//    typedef QRect(*fptr)(QListView *, QModelIndex);
//    fptr visual = (fptr)(&QListView::visualRect);
//    Stub stub1;
//    stub1.set(ADDR(SimpleListView, hasFocus), stub_focus_simpledelegateT);
//    m_simpleListDelegate->paint(painter, option, QModelIndex());
//}
