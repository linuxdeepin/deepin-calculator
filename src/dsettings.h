#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class DSettings : public QObject
{
    Q_OBJECT

public:
    DSettings(QObject *parent = nullptr);
    ~DSettings();

    QString configPath();
    QVariant getOption(const QString &key);
    void setOption(const QString &key, const QVariant &value);

private:
    QSettings *m_settings;
};

#endif
