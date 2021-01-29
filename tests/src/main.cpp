// test_main.cpp 测试入口

#include <QApplication>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QTest>
int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
