#include "ut_memorypublic.h"

#include "../../src/memorypublic.h"

Ut_MemoryPublic::Ut_MemoryPublic()
{

}

TEST_F(Ut_MemoryPublic, getwidget)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    ASSERT_EQ(memorywidget->m_calculatormode, 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryPublic, memoryplus)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    m_memoryPublic->memoryplus(Quantity(0));
    ASSERT_EQ(memorywidget->m_listwidget->count(), 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryPublic, memoryminus)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    m_memoryPublic->memoryminus(Quantity(0));
    ASSERT_EQ(memorywidget->m_listwidget->count(), 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryPublic, memoryclean)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    m_memoryPublic->generateData(Quantity(0));
    m_memoryPublic->generateData(Quantity(0));
    m_memoryPublic->generateData(Quantity(0));
    m_memoryPublic->memoryclean();
    ASSERT_EQ(memorywidget->m_listwidget->count(), 1);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryPublic, widgetplus)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    m_memoryPublic->generateData(Quantity(1));
    m_memoryPublic->widgetplus(0, Quantity(1));
    MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(memorywidget->m_listwidget->itemWidget(memorywidget->m_listwidget->item(0)));
    ASSERT_EQ(w1->textLabel(), "2");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryPublic, widgetminus)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    m_memoryPublic->generateData(Quantity(1));
    m_memoryPublic->widgetminus(0, Quantity(1));
    MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(memorywidget->m_listwidget->itemWidget(memorywidget->m_listwidget->item(0)));
    ASSERT_EQ(w1->textLabel(), "0");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryPublic, widgetclean)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    memorywidget->m_calculatormode = 0;
    m_memoryPublic->generateData(Quantity(0));
    m_memoryPublic->generateData(Quantity(0));
    m_memoryPublic->generateData(Quantity(0));
    m_memoryPublic->widgetclean(1, 1, false);
    ASSERT_EQ(memorywidget->m_listwidget->count(), 2);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_MemoryPublic, setThemeType)
{
    MemoryPublic *m_memoryPublic = new MemoryPublic;
    MemoryWidget *memorywidget = m_memoryPublic->getwidget(MemoryPublic::memorymode::scientificright);
    m_memoryPublic->m_list.clear();
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::LightType);
    m_memoryPublic->setThemeType(0);
    DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::ColorType::DarkType);
    m_memoryPublic->setThemeType(0);
    ASSERT_EQ(memorywidget->m_clearbutton->m_currentUrl, ":/assets/images/dark/empty_normal.svg");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}
