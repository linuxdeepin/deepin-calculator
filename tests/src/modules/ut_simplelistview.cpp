#include "ut_simplelistview.h"
#define private public
#define protected public
#include "src/modules/simplelistview.h"
#undef private
#undef protected

Ut_SimpleListView::Ut_SimpleListView()
{

}

TEST_F(Ut_SimpleListView, contextMenuEvent)
{
    SimpleListView *m_simpleListView = new SimpleListView();
    m_simpleListView->contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Reason::Mouse, m_simpleListView->pos(), m_simpleListView->pos(), Qt::KeyboardModifier::NoModifier));
    //无ASSERT
    DSettings::deleteInstance();
}

TEST_F(Ut_SimpleListView, mouseMoveEvent)
{
    SimpleListView *m_simpleListView = new SimpleListView();
    m_simpleListView->m_mode = 1;
    m_simpleListView->m_itemfill = true;
    m_simpleListView->m_ispressed = true;
    m_simpleListView->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_simpleListView->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_simpleListView->m_ispressed = false;
    m_simpleListView->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_simpleListView->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_simpleListView->m_mode = 0;
    m_simpleListView->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, m_simpleListView->pos(), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    m_simpleListView->mouseMoveEvent(new QMouseEvent(QMouseEvent::Type::MouseMove, QPoint(m_simpleListView->pos().x() + 640, m_simpleListView->pos().y()), Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier));
    //无ASSERT
    DSettings::deleteInstance();
}

TEST_F(Ut_SimpleListView, adjustScrollbarMargins)
{
    SimpleListView *m_simpleListView = new SimpleListView();
    m_simpleListView->adjustScrollbarMargins();
    //无ASSERT
    DSettings::deleteInstance();
}

TEST_F(Ut_SimpleListView, keyPressEvent)
{
    SimpleListView *m_simpleListView = new SimpleListView();
    SimpleListModel *m_simpleListModel = new SimpleListModel();
    m_simpleListModel->appendText("1", true);
    m_simpleListModel->appendText("2", true);
    m_simpleListModel->appendText("3", true);
    m_simpleListView->setModel(m_simpleListModel);
    m_simpleListView->setCurrentIndex(m_simpleListModel->index(0, 0));
    m_simpleListView->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    m_simpleListView->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    m_simpleListView->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
    m_simpleListView->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier));
    //无ASSERT
    DSettings::deleteInstance();
}
