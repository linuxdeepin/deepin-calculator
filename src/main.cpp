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

#include <QDBusInterface>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QSettings>
#include <DApplication>
#include <DApplicationSettings>
#include <DGuiApplicationHelper>
#include <DLog>
#include <DWidgetUtil>
#include <DWindowManagerHelper>

#include "mainwindow.h"
#include "environments.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE
static QString g_appPath;  //全局路径
static bool oldversion = false; //旧版本升级新版本

//20200702 edit for bug-36389,旧版本有设置过的第一次读取后不再读取themetype.cfg
//获取配置文件主题类型，并重新设置
DGuiApplicationHelper::ColorType getThemeTypeSetting()
{
    //需要找到自己程序的配置文件路径，并读取配置，这里只是用home路径下themeType.cfg文件举例,具体配置文件根据自身项目情况
    QString t_appDir = g_appPath + QDir::separator() + "themetype.cfg";
    QFile t_configFile(t_appDir);
    int t_readType = 0;
    if (t_configFile.exists()) {
        t_configFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray t_readBuf = t_configFile.readAll();
        t_readType = QString(t_readBuf).toInt();

        t_configFile.remove();
        oldversion = true;
    }

    //获取读到的主题类型，并返回设置
    switch (t_readType) {
    case 0:
        // 跟随系统主题
        return DGuiApplicationHelper::UnknownType;
    case 1:
        // 浅色主题
        return DGuiApplicationHelper::LightType;

    case 2:
        // 深色主题
        return DGuiApplicationHelper::DarkType;
    default:
        return DGuiApplicationHelper::UnknownType;
    }
}

//保存当前主题类型配置文件
//void saveThemeTypeSetting(int type)
//{
//    //需要找到自己程序的配置文件路径，并写入配置，这里只是用home路径下themeType.cfg文件举例,具体配置文件根据自身项目情况
//    QString t_appDir = g_appPath + QDir::separator() + "themetype.cfg";
//    QFile t_configFile(t_appDir);

//    t_configFile.open(QIODevice::WriteOnly | QIODevice::Text);
//    //直接将主题类型保存到配置文件，具体配置key-value组合根据自身项目情况
//    QString t_typeStr = QString::number(type);
//    t_configFile.write(t_typeStr.toUtf8());
//    t_configFile.close();
//}

int main(int argc, char *argv[])
{
    // DApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    g_appPath = QDir::homePath() + QDir::separator() + "." + qApp->applicationName();
    QDir t_appDir;
    t_appDir.mkpath(g_appPath);
    app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-calculator");
    app.setApplicationAcknowledgementPage(
        "https://www.deepin.org/acknowledgments/deepin-calculator");
    QIcon t_icon = QIcon::fromTheme("deepin-calculator");
    app.setProductIcon(t_icon);
    app.setProductName(DApplication::translate("MainWindow", "Calculator"));
    app.setApplicationDescription(
        DApplication::translate("MainWindow",
                                "Calculator is an easy to use desktop calculator, supporting standard, scientific and programmer modes."));
    app.setApplicationVersion(VERSION);
    app.setQuitOnLastWindowClosed(true);
    app.setApplicationDisplayName(QObject::tr("Calculator"));
    // app.setStyle("chameleon");

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
    window.setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    DSettingsAlt *m_dsettings = DSettingsAlt::instance(&window);
    if (app.setSingleInstance(app.applicationName(), DApplication::UserScope)) {
        Dtk::Widget::moveToCenter(&window);
        m_dsettings->setOption("windowX", window.pos().x());
        m_dsettings->setOption("windowY", window.pos().y());
    } else {
        window.move(m_dsettings->getOption("windowX").toInt() + 10, m_dsettings->getOption("windowY").toInt() + 10);
    }

    DGuiApplicationHelper::ColorType oldpalette = getThemeTypeSetting();
    DApplicationSettings savetheme(&app);
    if (oldversion == true) {
        DGuiApplicationHelper::instance()->setThemeType(oldpalette);
    }


    // 应用已保存的主题设置
//    DGuiApplicationHelper::ColorType t_type = DGuiApplicationHelper::instance()->themeType();
//    saveThemeTypeSetting(t_type);
    //监听当前应用主题切换事件
//    QObject::connect(DGuiApplicationHelper::instance(),
//                     &DGuiApplicationHelper::paletteTypeChanged,
//    [](DGuiApplicationHelper::ColorType type) {
//        qDebug() << type;
//        // 保存程序的主题设置  type : 0,系统主题， 1,浅色主题， 2,深色主题
//        saveThemeTypeSetting(type);
//        DGuiApplicationHelper::instance()->setPaletteType(type);
//    });

    // 20200330 主题记忆更改为规范代码
//    DApplicationSettings savetheme(&app);
    // Register debus service.
    dbus.registerObject("/com/deepin/calculator", &window, QDBusConnection::ExportScriptableSlots);
    window.show();
    return app.exec();
}
