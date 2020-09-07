#include "ut_dsetting.h"
#define private public
#include "src/dsettings.h"
#undef private

Ut_DSetting::Ut_DSetting()
{

}

TEST_F(Ut_DSetting, getwidget)
{
    DSettings *m_dsetting = new DSettings;
    m_dsetting->m_settings->setValue("theme", "");
    m_dsetting->m_settings->setValue("mode", "");
    m_dsetting->m_settings->setValue("history", "");
    m_dsetting->m_settings->setValue("windowX", "");
    m_dsetting->m_settings->setValue("windowY", "");
    m_dsetting->m_settings->setValue("windowWidth", "");
    m_dsetting->m_settings->setValue("windowHeight", "");
    m_dsetting->initConfig();
    ASSERT_EQ(m_dsetting->getOption("windowHeight").toInt(), 0);
    DSettings::deleteInstance();
}
