#include "utils.h"
#include "abacus/Express.h"
#include "abacus/Logger.h"
#include "abacus/Session.h"
#include <QFile>

using namespace zhcosin;

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
    Express a;

    if (0 !=
        a.WordParse(expression, Session::GetInstance()->GetUserVariables(), Session::GetInstance()->GetUserFunctions())) {
        a.Destroy();
        return -1;
    }

    if (0 != a.SyntaxParse()) {
        a.Destroy();
        return -2;
    }

    double value = a.Value();

    return value;
}
