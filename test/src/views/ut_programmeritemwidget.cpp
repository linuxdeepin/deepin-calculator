#include "ut_programmeritemwidget.h"
#include "../../src/views/programmeritemwidget.h"
#include "../../src/dsettings.h"
#include "../../src/memorypublic.h"


Ut_ProgrammerItemWidget::Ut_ProgrammerItemWidget()
{

}

TEST_F(Ut_ProgrammerItemWidget, enterEvent)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->enterEvent(new QEvent(QEvent::Type::MouseMove));
    ASSERT_TRUE(m_programmerItemWidget->m_ishover);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgrammerItemWidget, leaveEvent)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->leaveEvent(new QEvent(QEvent::Type::MouseMove));
    ASSERT_FALSE(m_programmerItemWidget->m_ishover);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgrammerItemWidget, cleanHoverState)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->cleanHoverState();
    ASSERT_FALSE(m_programmerItemWidget->m_ishover);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgrammerItemWidget, cleanFocusState)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->cleanFocusState();
    ASSERT_FALSE(m_programmerItemWidget->m_isfocus);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgrammerItemWidget, isMarkHide)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->isMarkHide(true);
    ASSERT_TRUE(m_programmerItemWidget->m_isMarkHide);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgrammerItemWidget, initMark)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->initMark();
    ASSERT_EQ(m_programmerItemWidget->m_pointVec.at(1), m_programmerItemWidget->m_markmid);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgrammerItemWidget, themetypechanged)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->themetypechanged(1);
    ASSERT_EQ(m_programmerItemWidget->m_label->palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text).name(),
              "#000000");
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}

TEST_F(Ut_ProgrammerItemWidget, setFocus)
{
    ProgrammerItemWidget *m_programmerItemWidget = new ProgrammerItemWidget("WORD");
    m_programmerItemWidget->setFocus();
    ASSERT_TRUE(m_programmerItemWidget->m_isfocus);
    DSettingsAlt::deleteInstance();
    MemoryPublic::deleteInstance();
}
