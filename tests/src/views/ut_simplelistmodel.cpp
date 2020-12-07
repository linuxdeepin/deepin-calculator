#include "ut_simplelistmodel.h"
#define private public
#include "../../src/views/simplelistmodel.h"
#undef private

Ut_SimpleListModel::Ut_SimpleListModel()
{

}

TEST_F(Ut_SimpleListModel, data)
{
    SimpleListModel *m_simpleListModel = new SimpleListModel();
    m_simpleListModel->appendText("1", true);
    m_simpleListModel->appendText("2", true);
    m_simpleListModel->appendText("3", true);
    m_simpleListModel->data(m_simpleListModel->index(1, 0), 7);
    m_simpleListModel->data(m_simpleListModel->index(1, 0), SimpleListModel::Role::ExpressionCount);
    m_simpleListModel->data(m_simpleListModel->index(1, 0), SimpleListModel::Role::ExpressionPrevious);
    ASSERT_EQ(m_simpleListModel->data(m_simpleListModel->index(1, 0), SimpleListModel::Role::ExpressionNext), "3");
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SimpleListModel, updataList1)
{
    SimpleListModel *m_simpleListModel = new SimpleListModel();
    for (int i = 0; i < 500; i++) {
        m_simpleListModel->m_expressionList.append("1");
    }
    m_simpleListModel->updataList("2", 500, true);
    m_simpleListModel->updataList("3", 500, false);
    m_simpleListModel->updataList("3x3", 500, false);
    ASSERT_EQ(m_simpleListModel->m_expressionList.count(), 500);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SimpleListModel, updataList2)
{
    SimpleListModel *m_simpleListModel = new SimpleListModel();
    for (int i = 0; i < 500; i++) {
        m_simpleListModel->m_expressionList.append("1");
        m_simpleListModel->m_answerlist.append(Quantity(1));
    }
    m_simpleListModel->updataList(Quantity(9), "3x3", 500);
    ASSERT_EQ(m_simpleListModel->m_expressionList.count(), 500);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SimpleListModel, deleteItem)
{
    SimpleListModel *m_simpleListModel = new SimpleListModel();
    m_simpleListModel->m_expressionList.append("1");
    m_simpleListModel->m_answerlist.append(Quantity(1));
    m_simpleListModel->deleteItem(0);
    ASSERT_EQ(m_simpleListModel->m_expressionList.count(), 0);
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SimpleListModel, copyToClipboard)
{
    SimpleListModel *m_simpleListModel = new SimpleListModel();
    m_simpleListModel->m_expressionList.append("1");
    m_simpleListModel->copyToClipboard(0);
    //无ASSERT
    DSettingsAlt::deleteInstance();
}

TEST_F(Ut_SimpleListModel, getAnswer)
{
    SimpleListModel *m_simpleListModel = new SimpleListModel();
    m_simpleListModel->m_answerlist.append(Quantity(1));
    m_simpleListModel->getAnswer(1);
    ASSERT_EQ(m_simpleListModel->getAnswer(0), Quantity(1));
    DSettingsAlt::deleteInstance();
}
