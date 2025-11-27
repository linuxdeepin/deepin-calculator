// Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "utils.h"
#include "dsettings.h"
#include "../3rdparty/core/settings.h"

#include <QRegularExpression>
#include <QStandardPaths>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>

const static QString ATOE = "ABCDEF";

Utils::Utils()
{

}

Utils::~Utils()
{
}

/**
 * @brief 返回程序文件路径
 */
QString Utils::getConfigDir()
{
    qDebug() << "Enter getConfigDir()";
    QDir dir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first())
             .filePath(qApp->organizationName()));
    
    QString configPath = dir.filePath(qApp->applicationName());
    qDebug() << "Exit getConfigDir(), config path:" << configPath;
    return configPath;
}

/**
 * @brief 获取文件内容（暂未使用）
 */
QString Utils::getQssContent(const QString &filePath)
{
    qDebug() << "Enter getQssContent(), filePath:" << filePath;
    QFile file(filePath);
    QString qss = nullptr;

    if (file.open(QIODevice::ReadOnly)) {
        qss = file.readAll();
        qInfo() << "Successfully read QSS content from" << filePath;
    } else {
        qWarning() << "Failed to open QSS file:" << filePath << "Error:" << file.errorString();
    }

    qDebug() << "Exit getQssContent()";
    return qss;
}

/**
 * @brief 补充千位符(针对结果)
 */
QString Utils::formatThousandsSeparators(const QString &str)
{
    qDebug() << "Enter formatThousandsSeparators(), input:" << str;

    const auto sys = Settings::instance();
    const QString decSym = sys->getSystemDecimalSymbol();
    const QString grpSym = sys->getSystemDigitGroupingSymbol();
    const bool groupingEnabled = sys->getSystemDigitGrouping();
    const int separate = DSettingsAlt::instance()->getSeparate();

    qInfo() << "formatThousandsSeparators settings decSym=" << decSym << "grpSym=" << grpSym << "grouping=" << groupingEnabled;

    QString input = str;
    QString sign;
    if (!input.isEmpty() && (input.at(0) == QChar('+') || input.at(0) == QChar('-'))) {
        sign = input.left(1);
        input.remove(0, 1);
    }

    QString exponent;
    int eIndex = input.indexOf('E');
    if (eIndex >= 0) {
        exponent = input.mid(eIndex);
        input = input.left(eIndex);
    }

    // 使用系统小数符优先定位小数点；若无则回退'.'。切分后仅清理整数部分已有分组符，小数部分保持不变
    QString fractional;
    bool hasDecimalPoint = false;
    QString integerPart = input;
    {
        int decIdx = -1;
        // 如果系统小数符不是'.'，优先查找系统小数符
        if (!decSym.isEmpty() && decSym != QLatin1String(".")) {
            decIdx = input.lastIndexOf(decSym);
        }
        // 未找到系统小数符，或系统小数符就是'.'，则查找'.'
        if (decIdx < 0) {
            decIdx = input.lastIndexOf(QLatin1Char('.'));
        }
        if (decIdx >= 0) {
            hasDecimalPoint = true;
            fractional = input.mid(decIdx + 1);
            integerPart = input.left(decIdx);
        }
        // 仅清理整数部分中可能已存在的分组符号（'.'、','、全角'，'、空格、系统分组符），小数部分完全保持原样
        const QChar fullWidthComma(0xFF0C);
        if (!grpSym.isEmpty())
            integerPart.remove(grpSym);
        integerPart.remove(QLatin1Char('.'));
        integerPart.remove(QLatin1Char(','));
        integerPart.remove(fullWidthComma);
        integerPart.remove(QLatin1Char(' '));
    }
    
    QString groupedInteger = integerPart;
    if (groupingEnabled && !grpSym.isEmpty() && separate > 0 && integerPart.length() > separate) {
        for (int i = groupedInteger.length() - separate; i > 0; i -= separate) {
            groupedInteger.insert(i, grpSym);
        }
    }

    QString result = sign + groupedInteger;
    if (hasDecimalPoint) {
        QString displayDecimal = decSym.isEmpty() ? QStringLiteral(".") : decSym;
        result += displayDecimal;
        result += fractional;
    }
    result += exponent;

    qDebug() << "Exit formatThousandsSeparators(), output:" << result;
    return result;
}

/**
 * @brief 该string是否是数字
 */
bool Utils::stringIsDigit(const QString &str)
{
    qDebug() << "Enter stringIsDigit(), input:" << str;
    bool isDigit = true;

    for (auto &ch : str) {
        if (!ch.isDigit() && ch != '.' && ch != ',' && ch != '-') { //非数字条件为非数字小数点分隔符负号
            qDebug() << "Invalid character found:" << ch;
            isDigit = false;
            break;
        }
    }

    qDebug() << "Exit stringIsDigit(), result:" << isDigit;
    return isDigit;
}

/**
 * @brief 针对输入框内容重新设置千位符
 */
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

QString Utils::formatThousandsSeparatorsPro(const QString &str, const int Base)
{


    QString result = str;
    int startPos = result.indexOf(QRegularExpression("[0-9]"));
    int startPosHex = result.indexOf(QRegularExpression("[A-F0-9]"));
    switch (Base) {
    case 16:
    case 2:
        if (startPosHex >= 0) {
            int endPos = result.length();
            for (int i = endPos - 4; i >= startPosHex + 1; i -= 4) {
                result.insert(i, " ");
            }
        }
        break;
    case 10:
        return formatThousandsSeparators(str);
    case 8:
        if (startPos >= 0) {
            int endPos  = result.length();
            for (int i = endPos - 3; i >= startPos + 1; i -= 3) {
                result.insert(i, " ");
            }
        }
        break;
    default:
        break;
    }

    return result;
}

/**
 * @brief Utils::stringIsDigitPro
 * @param str:待判断字符串
 * @param Base:进制
 * @return 是否是数字
 * 程序员模式下判断字符串是否为数字
 */
bool Utils::stringIsDigitPro(const QString &str, const int Base)
{
    bool isDigit = true;

    switch (Base) {
    case 16:
        for (auto &ch : str) {
            if (!ch.isDigit() && ch != ' ' && !isAtoE(ch)) {
                isDigit = false;
                break;
            }
        }
        break;
    case 10:
        for (auto &ch : str) {
            if (!ch.isDigit() && ch != ',' && ch != '-') {
                isDigit = false;
                break;
            }
        }
        break;
    case 8:
    case 2:
        for (auto &ch : str) {
            if (!ch.isDigit() && ch != ' ') {
                isDigit = false;
                break;
            }
        }
        break;
    default:
        for (auto &ch : str) {
            if (!ch.isDigit() && ch != ',' && ch != '-') {
                isDigit = false;
                break;
            }
        }
        break;
    }
    return isDigit;
}

QString Utils::reformatSeparatorsPro(const QString &exp, const int Base)
{
    QString seg;
    QStringList expList;

    int count = 0;
    for (auto ch : exp) {
        if (ch.isDigit() || isAtoE(ch)) {
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
        if (stringIsDigitPro(item, Base)) {
            item = formatThousandsSeparatorsPro(item, Base);
        }
        formatStr.append(item);
    }

    for (int i = 0; i < formatStr.length();) {
        if (formatStr.at(i).isLower()) {
            if (formatStr.at(i) == 'n' && formatStr.at(i + 1) == 'a') {
                i += 4;
            } else if (formatStr.at(i) == 'o') {
                i += 2;
            } else {
                i += 3;
            }
            if (i <= formatStr.length() - 1) {
                formatStr.insert(i, " ");
                i++;
            }
        } else if (i < formatStr.length() - 1 && formatStr.at(i + 1).isLower()) {
            formatStr.insert(i + 1, " ");
            i += 2;
        } else {
            i++;
        }
    }

    return formatStr;
}

QString Utils::toHalfWidth(const QString &str)
{
    QString result = str;
    if (str.length() <= 0) {
        qDebug() << "Empty input string";
        return QString();
    }
    for (int i = 0; i < result.length(); i++) {
        int charIntValue = result.at(i).unicode();
        if (charIntValue >= 65281 && charIntValue <= 65374) {
            result.replace(i, 1, QChar(charIntValue - 65248));
        } else if (charIntValue == 12288) {
            result.replace(i, 1, QChar(32));
        }
    }
    return result;
}

bool Utils::isAtoE(const QChar &num)
{
    return ATOE.contains(num);
}
