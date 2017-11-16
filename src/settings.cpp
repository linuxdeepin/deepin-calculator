#include "settings.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

Settings::Settings(QObject *parent) : QObject(parent)
{
    settings = new QSettings(QDir(configPath()).filePath("config.conf"), QSettings::IniFormat);

    if (settings->value("theme").toString().isEmpty()) {
        setOption("theme", "light");
    }
}

Settings::~Settings()
{
    delete settings;
}

QString Settings::configPath()
{
    return QDir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first()).filePath(qApp->organizationName())).filePath(qApp->applicationName());
}

QString Settings::getOption(const QString &key)
{
    return settings->value(key).toString();
}

void Settings::setOption(const QString &key, const QString &value)
{
    settings->setValue(key, value);
}
