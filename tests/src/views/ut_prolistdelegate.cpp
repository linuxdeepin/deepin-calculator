#include "ut_prolistdelegate.h"

#include "../../src/views/prolistdelegate.h"
#include "../../src/views/prolistmodel.h"

Ut_ProListDelegate::Ut_ProListDelegate()
{

}

TEST_F(Ut_ProListDelegate, setThemeType)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    m_proListDelegate->setThemeType(1);
    ASSERT_EQ(m_proListDelegate->m_themeType, 1);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProListDelegate, currentfocusindex)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    m_proListDelegate->currentfocusindex(QModelIndex());
    ASSERT_EQ(m_proListDelegate->m_focusindex, QModelIndex());
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProListDelegate, sizeHint)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    ProListModel *model = new ProListModel();
    m_proListDelegate->sizeHint(QStyleOptionViewItem(), model->index(0, 0));
    ASSERT_EQ(m_proListDelegate->sizeHint(QStyleOptionViewItem(), model->index(3, 0)), QSize(451, 28));
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProListDelegate, editorEvent)
{
    ProListDelegate *m_proListDelegate = new ProListDelegate;
    ProListModel *model = new ProListModel();
    QStyleOptionViewItem item;
    ASSERT_TRUE(m_proListDelegate->editorEvent(new QEvent(QEvent::Type::None), model, item, model->index(0, 0)));
    DSettingsAlt::deleteInstance();
}
