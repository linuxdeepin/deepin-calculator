#ifndef UT_MEMORYPUBLIC_H
#define UT_MEMORYPUBLIC_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

#include "../../src/dsettings.h"

class Ut_MemoryPublic : public ::testing::Test
{
public:
    Ut_MemoryPublic();
};

#endif // UT_MEMORYPUBLIC_H
