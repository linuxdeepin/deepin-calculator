#ifndef UT_PROBITWIDGET_H
#define UT_PROBITWIDGET_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

class Ut_ProBitWidget : public ::testing::Test
{
public:
    Ut_ProBitWidget();

public:
    virtual void SetUp();

    virtual void TearDown();

protected:
};


#endif // UT_PROBITWIDGET_H
