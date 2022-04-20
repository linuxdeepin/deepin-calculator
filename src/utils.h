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

#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils : public QObject
{
    Q_OBJECT

public:
    Utils();
    ~Utils();

    static QString getConfigDir();
    static QString getQssContent(const QString &filePath);
    static QString formatThousandsSeparators(const QString &str);
    static bool stringIsDigit(const QString &str);
    static QString reformatSeparators(const QString &exp);

    //程序员计算器format
    static QString formatThousandsSeparatorsPro(const QString &str, const int Base);
    static bool stringIsDigitPro(const QString &str, const int Base);
    static QString reformatSeparatorsPro(const QString &exp, const int Base);

    //全角转半角符号
    static QString toHalfWidth(const QString &str);

private:
    static bool isAtoE(const QChar &num);
};

#endif
