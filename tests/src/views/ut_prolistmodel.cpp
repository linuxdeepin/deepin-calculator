#include "ut_prolistmodel.h"

#include "../../src/views/prolistmodel.h"
#include "../../3rdparty/math/quantity.h"

Ut_ProListModel::Ut_ProListModel()
{

}

TEST_F(Ut_ProListModel, data)
{
    ProListModel *m_proListModel = new ProListModel();
    m_proListModel->updataList(Quantity(3));
    m_proListModel->data(m_proListModel->index(1, 0), ProListModel::Role::ExpressionCount);
    m_proListModel->data(m_proListModel->index(1, 0), ProListModel::Role::ExpressionPrevious);
    ASSERT_EQ(m_proListModel->data(m_proListModel->index(1, 0), ProListModel::Role::ExpressionNext), "3");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProListModel, clearItems)
{
    ProListModel *m_proListModel = new ProListModel();
    m_proListModel->clearItems();
    ASSERT_EQ(m_proListModel->m_expressionList.at(3), "");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_ProListModel, updataList)
{
    ProListModel *m_proListModel = new ProListModel();
    m_proListModel->updataList(Quantity(11));
    ASSERT_TRUE(m_proListModel->m_expressionList.at(0) == "B");
    ASSERT_TRUE(m_proListModel->m_expressionList.at(1) == "11");
    ASSERT_TRUE(m_proListModel->m_expressionList.at(2) == "13");
    ASSERT_TRUE(m_proListModel->m_expressionList.at(3) == "1011");
    DSettingsAlt::deleteInstance();
}


