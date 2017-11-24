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
        int endPos = result.indexOf(QRegularExpression("[.]"));

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
        if (!ch.isDigit()) {\
            isDigit = false;
            break;
        }
    }

    return isDigit;
}
