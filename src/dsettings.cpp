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
#include "utils.h"
#include <QDir>
#include <DApplication>
#include <QDesktopWidget>

DWIDGET_USE_NAMESPACE
static DSettings *INSTANCE = nullptr;

DSettings *DSettings::instance(QObject *p)
{
    if (!INSTANCE) {
        INSTANCE = new DSettings(p);
    }

    return INSTANCE;
}

DSettings::DSettings(QObject *parent)
    : QObject(parent),
      m_settings(new QSettings(QDir(Utils::getConfigDir()).filePath("config.config"), QSettings::IniFormat, this))
{
    initConfig();
}

DSettings::~DSettings()
{
//    delete m_settings;
}

void DSettings::initConfig()
{
    if (m_settings->value("theme").toString().isEmpty()) {
        setOption("theme", "light");
    }

    if (m_settings->value("mode").toString().isEmpty()) {
        setOption("mode", 0);
    }

    if (m_settings->value("history").toString().isEmpty()) {
        setOption("history", 0);
    }
    if (m_settings->value("windowX").toString().isEmpty()) {
        setOption("windowX", 0);
    }
    if (m_settings->value("windowY").toString().isEmpty()) {
        setOption("windowY", 0);
    }
}

QVariant DSettings::getOption(const QString &key)
{
    return m_settings->value(key);
}

void DSettings::setOption(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}
