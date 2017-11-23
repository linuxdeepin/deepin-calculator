#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class DSettings : public QObject
{
    Q_OBJECT

public:
    DSettings(QObject *parent=0);
    ~DSettings();

    QString configPath();
    QString getOption(const QString &key);
    void setOption(const QString &key, const QString &value);

private:
    QSettings *m_settings;
};

#endif
