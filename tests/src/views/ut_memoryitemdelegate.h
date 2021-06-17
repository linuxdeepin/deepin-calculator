#ifndef UT_MEMORYITEMDELEGATE_H
#define UT_MEMORYITEMDELEGATE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>
#include <DApplicationHelper>
#include"../stub.h"
#include "../../src/views/memoryitemdelegate.h"

class Ut_MemoryItemDelegate : public ::testing::Test
{
public:
    Ut_MemoryItemDelegate();
    MemoryItemDelegate *m_memoryItemDelegate;

protected:
    void SetUp();
    void TearDown();
};

#endif // UT_MEMORYITEMDELEGATE_H
