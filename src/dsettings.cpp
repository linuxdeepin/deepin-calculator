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

#include "dsettings.h"

#include <QDir>
#include <QDesktopWidget>
#include <QDebug>
#include <DApplication>

#include "utils.h"

DWIDGET_USE_NAMESPACE
static DSettingsAlt *INSTANCE = nullptr;

/**
 * @brief 初始化
 */
DSettingsAlt *DSettingsAlt::instance(QObject *p)
{
    if (!INSTANCE) {
        INSTANCE = new DSettingsAlt(p);
    }

    return INSTANCE;
}

/**
 * @brief 相关信息保存在config.config文件中
 */
DSettingsAlt::DSettingsAlt(QObject *parent)
    : QObject(parent),
      m_settings(new QSettings(QDir(Utils::getConfigDir()).filePath("config.config"), QSettings::IniFormat, this))
{
    initConfig();;
}

DSettingsAlt::~DSettingsAlt()
{
//    delete m_settings;
}

void DSettingsAlt::initConfig()
{
    if (m_settings->value("theme").toString().isEmpty()) {
        setOption("theme", "light"); //主题初始化为浅色
    }

    if (m_settings->value("mode").toString().isEmpty()) {
        setOption("mode", 0); //模式初始化为标准模式
    }

    if (m_settings->value("history").toString().isEmpty()) {
        setOption("history", 0); //历史记录初始化关闭
    }
    if (m_settings->value("windowX").toString().isEmpty()) {
        setOption("windowX", 0);  //保存当前打开位置x
    }
    if (m_settings->value("windowY").toString().isEmpty()) {
        setOption("windowY", 0); //保存当前打开位置y
    }
    if (m_settings->value("windowWidth").toString().isEmpty()) {
        setOption("windowWidth", 0); //保存窗口width
    }
    if (m_settings->value("windowHeight").toString().isEmpty()) {
        setOption("windowHeight", 0); //保存窗口height
    }
}

/**
 * @brief 获取相关参数
 */
QVariant DSettingsAlt::getOption(const QString &key)
{
    return m_settings->value(key);
}

/**
 * @brief 以相关键值保存参数
 * @param key 键
 * @param value 值
 */
void DSettingsAlt::setOption(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}

void DSettingsAlt::deleteInstance()
{
    delete INSTANCE;
    INSTANCE = nullptr;
}
