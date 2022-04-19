#ifndef UT_PROGRAMMODULE_H
#define UT_PROGRAMMODULE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QTest>

#include "../../src/memorypublic.h"
#include "../../src/dsettings.h"
#include "../stub.h"
#include "../3rdparty/core/settings.h"

class Ut_ProgramModule : public ::testing::Test
{
public:
    Ut_ProgramModule();

protected:
};

#endif // UT_PROGRAMMODULE_H
