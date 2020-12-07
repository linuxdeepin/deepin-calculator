/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class DSettingsAlt : public QObject
{
    Q_OBJECT
private:
    explicit DSettingsAlt(QObject *parent = nullptr);
public:
    static DSettingsAlt *instance(QObject *p);

    ~DSettingsAlt();

    QVariant getOption(const QString &key);
    void setOption(const QString &key, const QVariant &value);
    static void deleteInstance();

private:
    void initConfig();

private:
    QSettings *m_settings;
};

#endif
