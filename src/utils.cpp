#include "utils.h"
#include <QFile>
#include <QRegularExpression>

Utils::Utils()
{

}

Utils::~Utils()
{
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
