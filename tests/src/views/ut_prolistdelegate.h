#ifndef UT_PROLISTDELEGATE_H
#define UT_PROLISTDELEGATE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

#include <QPainter>
#include "../../src/views/prolistdelegate.h"
#include "../../src/views/prolistmodel.h"
#include "../../src/views/prolistview.h"
#include"../stub.h"

class Ut_ProListDelegate : public ::testing::Test
{
public:
    Ut_ProListDelegate();

    ProListView *m_prolistview1 = new ProListView();
    ProListModel *m_model1 = new ProListModel();
    ProListDelegate *m_proListDelegate1 = new ProListDelegate();
};

#endif // UT_PROLISTDELEGATE_H
