#include "dsettings.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

DSettings::DSettings(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings(QDir(configPath()).filePath("config.conf"), QSettings::IniFormat);

    if (m_settings->value("theme").toString().isEmpty()) {
        setOption("theme", "light");
    }
}

DSettings::~DSettings()
{
    delete m_settings;
}

QString DSettings::configPath()
{
    return QDir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first()).filePath(qApp->organizationName())).filePath(qApp->applicationName());
}

QString DSettings::getOption(const QString &key)
{
    return m_settings->value(key).toString();
}

void DSettings::setOption(const QString &key, const QString &value)
{
    m_settings->setValue(key, value);
}
