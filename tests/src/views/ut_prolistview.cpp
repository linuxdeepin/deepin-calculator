#include "ut_prolistview.h"

#include "../../src/views/prolistview.h"

Ut_ProListView::Ut_ProListView()
{

}

TEST_F(Ut_ProListView, keyPressEvent)
{
    ProListView *m_proListView = new ProListView();
    ProListModel *m_proListModel = new ProListModel();
    ProListDelegate *m_prolistdegate = new ProListDelegate();
    m_proListView->setModel(m_proListModel);
    m_proListView->setItemDelegate(m_prolistdegate);
    m_proListModel->updataList(Quantity(5));
    m_proListView->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    m_proListView->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
    m_proListView->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    //无ASSERT
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProListView, focusIndex)
{
    ProListView *m_proListView = new ProListView();
    ProListModel *m_proListModel = new ProListModel();
    ProListDelegate *m_prolistdegate = new ProListDelegate();
    m_proListView->setModel(m_proListModel);
    m_proListView->setItemDelegate(m_prolistdegate);
    m_proListModel->updataList(Quantity(5));
    m_proListView->focusIndex();
    ASSERT_EQ(m_proListView->focusIndex(), m_proListView->model()->index(m_proListView->m_focusrow, 0));
    //无ASSERT
    DSettingsAlt::deleteInstance();
}

QAction *stub_exec_pro(const QPoint &pos, QAction *at = nullptr)
{
    Q_UNUSED(pos)
    Q_UNUSED(at)

    return nullptr;
}

TEST_F(Ut_ProListView, contextMenuEvent)
{
    ProListView *m_proListView = new ProListView;
    Stub stub;
    stub.set((QAction * (QMenu::*)(const QPoint &, QAction *))ADDR(QMenu, exec), stub_exec_pro);
//    QContextMenuEvent *e = new QContextMenuEvent(QContextMenuEvent::Reason::Mouse, m_proListView->pos());
    m_proListView->contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Reason::Mouse,
                                                          m_proListView->pos(), m_proListView->pos(),
                                                          Qt::KeyboardModifier::NoModifier));
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProListView, showTextEditMenuByAltM)
{
    ProListView *m_proListView = new ProListView;
    Stub stub;
    stub.set((QAction * (QMenu::*)(const QPoint &, QAction *))ADDR(QMenu, exec), stub_exec_pro);
//    QContextMenuEvent *e = new QContextMenuEvent(QContextMenuEvent::Reason::Mouse, m_proListView->pos());
    m_proListView->showTextEditMenuByAltM(QModelIndex());
    DSettingsAlt::deleteInstance();
}
