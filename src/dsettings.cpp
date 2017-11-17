#include "dsettings.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

DSettings::DSettings(QObject *parent) : QObject(parent)
{
    settings = new QSettings(QDir(configPath()).filePath("config.conf"), QSettings::IniFormat);

    if (settings->value("theme").toString().isEmpty()) {
        setOption("theme", "light");
    }
}

DSettings::~DSettings()
{
    delete settings;
}

QString DSettings::configPath()
{
    return QDir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first()).filePath(qApp->organizationName())).filePath(qApp->applicationName());
}

QString DSettings::getOption(const QString &key)
{
    return settings->value(key).toString();
}

void DSettings::setOption(const QString &key, const QString &value)
{
    settings->setValue(key, value);
}
