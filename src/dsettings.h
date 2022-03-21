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
    ~DSettingsAlt();
public:
    static DSettingsAlt *instance();

    QVariant getOption(const QString &key);
    void setOption(const QString &key, const QVariant &value);
    static void deleteInstance();

    int getStandardSeparate(); //获取标准计算器分隔位数
    void setStandardSeparate(int separate); //设置标准计算器分隔位数
    int getScientificSeparate(); //获取科学计算器分隔位数
    void setScientificSeparate(int separate); //设置科学计算器分隔位数
    int getProgrammerSeparate(); //获取程序员计算器分隔位数
    void setProgrammerSeparate(int separate); //设置程序员计算器分隔位数

private:
    void initConfig();
    void loadConfig();
    void saveConfig();

private:
    QSettings *m_settings;

    int m_standardSeparate = 3;   //标准计算器分隔位数
    int m_scientificSeparate = 3; //科学计算器分隔位数
    int m_programmerSeparate = 3; //程序员计算器分隔位数
};

#endif
