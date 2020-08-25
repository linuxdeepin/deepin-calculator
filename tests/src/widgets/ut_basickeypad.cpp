#include "ut_basickeypad.h"
#define private public
#include "src/widgets/basickeypad.h"
#undef private

Ut_BasicKeypad::Ut_BasicKeypad()
{

}

TEST_F(Ut_BasicKeypad, setKeyPress)
{
    BasicKeypad *m_basickeypad = new BasicKeypad;
//    m_basicmodul->setKeyPress(new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier));
//    ASSERT_EQ(m_basicmodul->findChild<InputEdit *>()->text(), "1");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}
