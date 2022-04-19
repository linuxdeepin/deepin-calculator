#ifndef UT_SIMPLELISTVIEW_H
#define UT_SIMPLELISTVIEW_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>
#include <QMenu>

#include"../stub.h"
#include "../../src/dsettings.h"

class Ut_SimpleListView : public ::testing::Test
{
public:
    Ut_SimpleListView();
};

#endif // UT_SIMPLELISTVIEW_H
