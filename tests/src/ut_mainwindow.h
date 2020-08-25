#ifndef UT_QTEST_H
#define UT_QTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

#include "src/mainwindow.h"
#include "src/memorypublic.h"
#include "src/dsettings.h"

class TestCala : public::testing::Test
{
public:
    TestCala();

public:
    virtual void SetUp() override;

    virtual void TearDown() override;

protected:
    MainWindow *m_mainwindow;
};

#endif // UT_QTEST_H
