/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDate>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QDBusInterface>
#include <DApplication>
#include <DWidgetUtil>
#include <DGuiApplicationHelper>
#include <DWindowManagerHelper>
#include <DLog>
#include "mainwindow.h"

DWIDGET_USE_NAMESPACE
static QString g_appPath;//全局路径

///获取配置文件主题类型，并重新设置
DGuiApplicationHelper::ColorType getThemeTypeSetting()
{
    //需要找到自己程序的配置文件路径，并读取配置，这里只是用home路径下themeType.cfg文件举例,具体配置文件根据自身项目情况
    QString t_appDir = g_appPath + QDir::separator() + "themetype.cfg";
    QFile t_configFile(t_appDir);

    t_configFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t_readBuf = t_configFile.readAll();
    int t_readType = QString(t_readBuf).toInt();

    //获取读到的主题类型，并返回设置
    switch (t_readType) {
    case 0:
        // 跟随系统主题
        return DGuiApplicationHelper::UnknownType;
    case 1:
//        浅色主题
        return DGuiApplicationHelper::LightType;

    case 2:
//        深色主题
        return DGuiApplicationHelper::DarkType;
    default:
        // 跟随系统主题
        return DGuiApplicationHelper::UnknownType;
    }
}

//保存当前主题类型配置文件
void saveThemeTypeSetting(int type)
{
    //需要找到自己程序的配置文件路径，并写入配置，这里只是用home路径下themeType.cfg文件举例,具体配置文件根据自身项目情况
    QString t_appDir = g_appPath + QDir::separator() + "themetype.cfg";
    QFile t_configFile(t_appDir);

    t_configFile.open(QIODevice::WriteOnly | QIODevice::Text);
    //直接将主题类型保存到配置文件，具体配置key-value组合根据自身项目情况
    QString t_typeStr = QString::number(type);
    t_configFile.write(t_typeStr.toUtf8());
    t_configFile.close();
}

int main(int argc, char *argv[])
{
    //DApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
//    DApplication::loadDXcbPlugin();
    qputenv("QT_WAYLAND_SHELL_INTEGRATION", "kwayland-shell");
    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    g_appPath = QDir::homePath() + QDir::separator() + "." + qApp->applicationName();
    QDir t_appDir;
    t_appDir.mkpath(g_appPath);
    app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-calculator");
    app.setApplicationVersion(DApplication::buildVersion("1.0.1"));
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-calculator");
    QIcon t_icon = QIcon::fromTheme("deepin-calculator");
    //app.setProductIcon(QIcon(":/images/deepin-calculator.svg"));
    app.setProductIcon(t_icon);
    app.setProductName(DApplication::translate("MainWindow", "Calculator"));
    app.setApplicationDescription(DApplication::translate("MainWindow", "Calculator is a simple and easy to use desktop calculator. It supports addition, subtraction, multiplication and division."));
    static const QDate buildDate = QLocale(QLocale::English).toDate(QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    QString t_date = buildDate.toString("MMdd");
    // Version Time
    app.setApplicationVersion(DApplication::buildVersion(t_date));
    //app.setTheme("light");
    app.setQuitOnLastWindowClosed(true);
    app.setApplicationDisplayName(QObject::tr("Calculator"));
    //app.setStyle("chameleon");

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerService("com.deepin.calculator");
    using namespace Dtk::Core;
    Dtk::Core::DLogManager::registerConsoleAppender();
    Dtk::Core::DLogManager::registerFileAppender();
    QCommandLineParser cmdParser;
    cmdParser.setApplicationDescription("deepin-calculator");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.process(app);

    MainWindow window;
    //start fix bug-37751
//    window.setWindowFlags(window.windowFlags() & ~ Qt::WindowMaximizeButtonHint);
//    window.setAttribute(Qt::WA_NativeWindow);
//    window.windowHandle()->setProperty("_d_dwayland_minimizable", true);
//    window.windowHandle()->setProperty("_d_dwayland_maximizable", false);
//    window.windowHandle()->setProperty("_d_dwayland_resizable", false);
    window.setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    //end fix
    Dtk::Widget::moveToCenter(&window);
    DGuiApplicationHelper::instance()->setPaletteType(getThemeTypeSetting());
    // 应用已保存的主题设置
    DGuiApplicationHelper::ColorType t_type = DGuiApplicationHelper::instance()->themeType();
    //saveThemeTypeSetting(t_type);
    //监听当前应用主题切换事件
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged,
    [](DGuiApplicationHelper::ColorType type) {
        qDebug() << type;
        // 保存程序的主题设置  type : 0,系统主题， 1,浅色主题， 2,深色主题
        saveThemeTypeSetting(type);
        DGuiApplicationHelper::instance()->setPaletteType(type);
    });

    // Register debus service.
    dbus.registerObject("/com/deepin/calculator", &window, QDBusConnection::ExportScriptableSlots);
    window.show();
    return app.exec();
}
