#ifndef UT_PROLISTMODEL_H
#define UT_PROLISTMODEL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

#include "../../src/dsettings.h"
#include "../../src/views/prolistmodel.h"

class Ut_ProListModel : public ::testing::Test
{
public:
    Ut_ProListModel();
    ProListModel *m_proListModel;

protected:
    void SetUp();
    void TearDown();
};

#endif // UT_PROLISTMODEL_H
