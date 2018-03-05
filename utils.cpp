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

#include <QRegularExpression>
#include <QStandardPaths>
#include <QApplication>
#include <QFile>
#include <QDir>

#include "utils.h"

Utils::Utils()
{

}

Utils::~Utils()
{
}

QString Utils::getConfigDir()
{
    QDir dir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first())
             .filePath(qApp->organizationName()));

    return dir.filePath(qApp->applicationName());
}

QString Utils::getQssContent(const QString &filePath)
{
    QFile file(filePath);
    QString qss = nullptr;

    if (file.open(QIODevice::ReadOnly)) {
        qss = file.readAll();
    }

    return qss;
}

QString Utils::formatThousandsSeparators(const QString &str)
{
    QString result = str;
    int startPos = result.indexOf(QRegularExpression("[0-9]"));
    if (startPos >= 0) {
        int endPos = result.indexOf('.');

        if (endPos < 0) {
            endPos = result.length();
        }

        for (int i = endPos - 3; i >= startPos + 1; i -= 3) {
            result.insert(i, ",");
        }
    }

    return result;
}

bool Utils::stringIsDigit(const QString &str)
{
    bool isDigit = true;

    for (auto &ch : str) {
        if (!ch.isDigit() && ch != '.' && ch != ',' && ch != '-') {
            isDigit = false;
            break;
        }
    }

    return isDigit;
}

QString Utils::reformatSeparators(const QString &exp)
{
    QString seg;
    QStringList expList;

    int count = 0;
    for (auto ch : exp) {
        if (ch.isDigit() || ch == '.') {
            seg.append(ch);
        } else {
            expList << seg;
            seg.clear();
            seg.append(ch);
            expList << seg;
            seg.clear();
        }

        if (count == exp.count() - 1) {
            expList << seg;
        }

        ++count;
    }

    QString formatStr;
    for (auto item : expList) {
        if (stringIsDigit(item)) {
            item = formatThousandsSeparators(item);
        }

        formatStr.append(item);
    }

    return formatStr;
}
