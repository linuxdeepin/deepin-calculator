#include "ut_qtest.h"
#define private public

TestCala::TestCala()
{

}

void TestCala::SetUp()
{
    m_mainwindow = new MainWindow;
}

void TestCala::TearDown()
{
    delete m_mainwindow;
}

TEST_F(TestCala, mainwindow)
{
//    BasicKeypad m_basicmodul;
//    qDebug() << "mainwindow";
//    DSettings::deleteInstance();
//    MemoryPublic::deleteInstance();
//    MainWindow *m_mainwindow = new MainWindow;
    QTest::mouseClick(m_mainwindow->findChild<BasicKeypad *>()->button(BasicKeypad::Key_1), Qt::LeftButton);
    QTest::mouseClick(m_mainwindow->findChild<BasicKeypad *>()->button(BasicKeypad::Key_Plus), Qt::LeftButton);
    QTest::mouseClick(m_mainwindow->findChild<BasicKeypad *>()->button(BasicKeypad::Key_3), Qt::LeftButton);
    QTest::mouseClick(m_mainwindow->findChild<BasicKeypad *>()->button(BasicKeypad::Key_Equals), Qt::LeftButton);
    ASSERT_EQ(m_mainwindow->findChild<InputEdit *>()->text(), "4");
    DSettings::deleteInstance();
    MemoryPublic::deleteInstance();
}

//TEST_F(TestCala, keypress)
//{
//    qDebug() << "keypress";
//    QTest::keyPress(m_mainwindow->findChild<InputEdit *>(), Qt::Key_1);
//    QTest::keyPress(m_mainwindow->findChild<InputEdit *>(), Qt::Key_Plus);
//    QTest::keyPress(m_mainwindow->findChild<InputEdit *>(), Qt::Key_2);
//    QTest::keyPress(m_mainwindow->findChild<InputEdit *>(), Qt::Key_Equal);
//    ASSERT_EQ(m_mainwindow->findChild<InputEdit *>()->text(), "3");
//}

