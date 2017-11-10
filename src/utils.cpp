#include "utils.h"
#include "abacus/Expression.h"
#include <QFile>

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

double Utils::compute(const std::string &expression)
{
    Expression e(expression, 10);

    try {
        return e.getResult();
    } catch (runtime_error err) {
        //qDebug() << err.what();
    }
}
