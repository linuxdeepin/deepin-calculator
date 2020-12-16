#ifndef UT_ARROWRECTANGLE_H
#define UT_ARROWRECTANGLE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

class Ut_ArrowRectangle : public ::testing::Test
{
public:
    Ut_ArrowRectangle();

public:
    virtual void SetUp();

    virtual void TearDown();

protected:
};

#endif // UT_ARROWRECTANGLE_H
