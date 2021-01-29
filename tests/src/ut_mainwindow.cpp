#include "ut_mainwindow.h"

#include "../../src/mainwindow.h"

#include <QPalette>

TestCala::TestCala()
{

}

TEST_F(TestCala, initTheme)
{
    MainWindow m_mainwindow;
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_mainwindow.initTheme();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::UnknownType);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_mainwindow.initTheme();
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(TestCala, initModule)
{
    MainWindow m_mainwindow;
    m_mainwindow.m_settings->setOption("mode", 2);
    m_mainwindow.initModule();
    m_mainwindow.m_settings->setOption("mode", 0);
    m_mainwindow.initModule();
    m_mainwindow.m_settings->setOption("mode", 1);
    m_mainwindow.initModule();
    ASSERT_FALSE(m_mainwindow.m_isinit);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(TestCala, switchToSimpleMode)
{
    MainWindow m_mainwindow;
    m_mainwindow.m_settings->setOption("mode", 1);
    m_mainwindow.switchToSimpleMode();
    ASSERT_EQ(m_mainwindow.m_settings->getOption("mode"), 0);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(TestCala, switchToScientificMode)
{
    MainWindow m_mainwindow;
    m_mainwindow.m_settings->setOption("mode", 0);
    m_mainwindow.switchToScientificMode();
    ASSERT_EQ(m_mainwindow.m_settings->getOption("mode"), 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(TestCala, keyPressEvent)
{
    MainWindow m_mainwindow;
    m_mainwindow.keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    //无ASSERT
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(TestCala, moveEvent)
{
    MainWindow m_mainwindow;
    m_mainwindow.moveEvent(new QMoveEvent(QPoint(1, 1), QPoint(0, 0)));
    ASSERT_EQ(m_mainwindow.m_settings->getOption("windowX"), 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(TestCala, resizeEvent)
{
    MainWindow m_mainwindow;
    m_mainwindow.switchToScientificMode();
    m_mainwindow.resizeEvent(new QResizeEvent(QSize(811, 542), QSize(451, 542)));
    ASSERT_EQ(m_mainwindow.m_settings->getOption("windowWidth"), 811);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

