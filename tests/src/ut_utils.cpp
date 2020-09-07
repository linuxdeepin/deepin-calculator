#include "ut_utils.h"
#define private public
#include "src/utils.h"
#undef private

Ut_Utils::Ut_Utils()
{

}

TEST_F(Ut_Utils, getwidget)
{
    Utils *m_utils = new Utils;
    m_utils->getConfigDir();
    //无ASSERT
//    ASSERT_EQ(m_dsetting->getOption("windowHeight").toInt(), 0);
}

TEST_F(Ut_Utils, getQssContent)
{
    Utils *m_utils = new Utils;
    m_utils->getQssContent("/etc/apt/sources.list");
    //无ASSERT
//    ASSERT_EQ(m_dsetting->getOption("windowHeight").toInt(), 0);
}

TEST_F(Ut_Utils, formatThousandsSeparators)
{
    Utils *m_utils = new Utils;
    m_utils->formatThousandsSeparators("1234");
    ASSERT_EQ(m_utils->formatThousandsSeparators("1234"), "1,234");
}
