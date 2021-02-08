#include "ut_probitwidget.h"
#include "../../src/widgets/probitwidget.h"


Ut_ProBitWidget::Ut_ProBitWidget()
{

}

TEST_F(Ut_ProBitWidget, setButtonEnabled)
{
    ProBitWidget *m_probitwidget = new ProBitWidget;
    m_probitwidget->setButtonEnabled(true);
    ASSERT_TRUE(m_probitwidget->m_firstbtn->isEnabled() && m_probitwidget->m_secondbtn->isEnabled()
                && m_probitwidget->m_thirdbtn->isEnabled() && m_probitwidget->m_fourthbtn->isEnabled());
}

TEST_F(Ut_ProBitWidget, getbutton)
{
    ProBitWidget *m_probitwidget = new ProBitWidget;
    ASSERT_EQ(m_probitwidget->getbutton(3), m_probitwidget->m_fourthbtn);
}
