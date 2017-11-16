#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings(QObject *parent=0);
    ~Settings();

    QString configPath();
    QString getOption(const QString &key);
    void setOption(const QString &key, const QString &value);

private:
    QSettings *settings;
};

#endif
