#ifndef UT_EXPRESSIONBAR_H
#define UT_EXPRESSIONBAR_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

#include "src/dsettings.h"
#include "src/modules/memorypublic.h"

class Ut_ExpressionBar : public ::testing::Test
{
public:
    Ut_ExpressionBar();

public:
    virtual void SetUp();

    virtual void TearDown();
};

#endif // UT_EXPRESSIONBAR_H
